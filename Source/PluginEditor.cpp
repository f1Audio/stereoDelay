/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TEXT_HEIGHT 20
#define INDENT 10
#define SLIDERSIZE 200
#define WIDTH SLIDERSIZE*3 +INDENT*5
#define HEIGHT TEXT_HEIGHT*11 + INDENT + SLIDERSIZE


//==============================================================================
StereoDelayAudioProcessorEditor::StereoDelayAudioProcessorEditor (StereoDelayAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    addAndMakeVisible(delayModeButton);
    delayModeButton.addItem("normal", 1);
    delayModeButton.addItem("ping-pong", 2);
    
    //change visibility of Invert Side Button + intensity slider
    delayModeButton.onChange = [this]{
        if(delayModeButton.getSelectedId() == 1){
            invertSideButton.setVisible(false);
            spreadSlider.setVisible(false);
        }else{
            invertSideButton.setVisible(true);
            spreadSlider.setVisible(true);
        }
    };
    delayModeAttachment.reset(new ComboBoxAttachment(valueTreeState, "delayMode", delayModeButton));
    
  
    
   
    
    
    addChildComponent(syncedDelayBox);
    syncedDelayBox.addItem("1/4", 1);
    syncedDelayBox.addItem("1/8", 2);
    syncedDelayBox.addItem("1/16", 3);
    syncedDelayBox.addItem("1/32", 4);
    
    syncedDelayAttachment.reset(new ComboBoxAttachment(valueTreeState, "timing", syncedDelayBox));
    
    addAndMakeVisible(syncButton);
    syncButton.setName("sync");
    
    //change visibility of delaybox + disable timeslider when sync enabled
    syncButton.onStateChange = [this]{
        if(syncButton.getToggleState() == true){
            timeSlider.setEnabled(false);
            syncedDelayBox.setVisible(true);
        }else{
            timeSlider.setEnabled(true);
            syncedDelayBox.setVisible(false);
        }
    };
    syncAttachment.reset(new ButtonAttachment(valueTreeState, "sync", syncButton));
   
    
    addChildComponent(invertSideButton);
    invertSideAttachment.reset(new ButtonAttachment(valueTreeState, "invert", invertSideButton));
    
    addAndMakeVisible(timeSlider);
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    timeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, TEXT_HEIGHT);
    timeSlider.setTextValueSuffix(" ms");
    timeAttachment.reset(new SliderAttachment(valueTreeState, "time", timeSlider));
    
    
    
    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, TEXT_HEIGHT);
    feedbackAttachment.reset(new SliderAttachment(valueTreeState, "feedback", feedbackSlider));
    
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, TEXT_HEIGHT);
    mixAttachment.reset(new SliderAttachment(valueTreeState, "mix", mixSlider));
    
    addChildComponent(spreadSlider);
    spreadSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    spreadSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight ,false, 80, TEXT_HEIGHT);
    spreadAttachment.reset(new SliderAttachment(valueTreeState, "spread", spreadSlider));
    
    
    //Labels
    
    addAndMakeVisible (timeLabel);
    timeLabel.setText ("Delay Time", juce::dontSendNotification);
    timeLabel.attachToComponent (&timeSlider, false);
    
    addAndMakeVisible(spreadLabel);
    spreadLabel.setText ("Intensity", juce::dontSendNotification);
    spreadLabel.attachToComponent (&spreadSlider, true);
    
    addAndMakeVisible (feedbackLabel);
    feedbackLabel.setText ("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent (&feedbackSlider, false);
    
    addAndMakeVisible (mixLabel);
    mixLabel.setText ("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent (&mixSlider, false);
    
    addAndMakeVisible (syncLabel);
    syncLabel.setText ("Sync", juce::dontSendNotification);
    syncLabel.attachToComponent (&syncButton, true);
    
    addAndMakeVisible (invertLabel);
    invertLabel.setText ("Invert Side", juce::dontSendNotification);
    invertLabel.attachToComponent (&invertSideButton, true);
    
  
    
    
    setSize (WIDTH, HEIGHT);
    //setResizable(true, true);
}

StereoDelayAudioProcessorEditor::~StereoDelayAudioProcessorEditor()
{
}

//==============================================================================
void StereoDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    //change background color
    g.fillAll (juce::Colours::blueviolet.brighter(0.1).withAlpha(0.7f));
    
    //change text color, size and centre it
    g.setColour (juce::Colours::whitesmoke);
    g.setFont (35.0f);
    g.drawFittedText ("Stereo Delay", getLocalBounds(), juce::Justification::centredTop, 1);
}

void StereoDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
   
    timeSlider.setBounds(INDENT, TEXT_HEIGHT*7, SLIDERSIZE, SLIDERSIZE);
    feedbackSlider.setBounds(timeSlider.getX()+ SLIDERSIZE, timeSlider.getY(), SLIDERSIZE, SLIDERSIZE);
    mixSlider.setBounds(feedbackSlider.getX() + SLIDERSIZE, feedbackSlider.getY(), SLIDERSIZE, SLIDERSIZE);
    delayModeButton.setBounds(feedbackSlider.getX() + SLIDERSIZE/4, 60, 125, 25);
    spreadSlider.setBounds(timeSlider.getX()+ SLIDERSIZE, timeSlider.getY()+ 1.1*SLIDERSIZE, SLIDERSIZE* 1.5, 50);
    syncButton.setBounds(timeSlider.getX()+40, 60, 25, 25);
    syncedDelayBox.setBounds(timeSlider.getX()+ 80, 60, 70, 25);
    invertSideButton.setBounds(mixSlider.getX()+ SLIDERSIZE, 60, 25, 25);


}
