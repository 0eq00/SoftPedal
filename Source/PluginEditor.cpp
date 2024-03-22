/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftPedalAudioProcessorEditor::SoftPedalAudioProcessorEditor (SoftPedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 300);
    startTimerHz(60);

    ch1Slider.setSliderStyle(juce::Slider::LinearHorizontal);
    ch1Slider.setRange(1, 16, 1);
    ch1Slider.setBounds(180, 20, 250, 20);
    ch1Slider.setValue(*(audioProcessor.ch1));
    addAndMakeVisible(&ch1Slider);

    ch1Label.setText("Ch1 - Soft Pedal OFF", juce::dontSendNotification);
    ch1Label.attachToComponent(&ch1Slider, true);

    ch2Slider.setSliderStyle(juce::Slider::LinearHorizontal);
    ch2Slider.setRange(1, 16, 1);
    ch2Slider.setBounds(180, 60, 250, 20);
    ch2Slider.setValue(*(audioProcessor.ch2));
    addAndMakeVisible(&ch2Slider);

    ch2Label.setText("Ch2 - Soft Pedal ON", juce::dontSendNotification);
    ch2Label.attachToComponent(&ch2Slider, true);

    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setRange(0, 127, 1);
    velocitySlider.setBounds(180, 100, 250, 20);
    velocitySlider.setValue(*(audioProcessor.vel));
    addAndMakeVisible(&velocitySlider);

    velocityLabel.setText("Velocity Decrease", juce::dontSendNotification);
    velocityLabel.attachToComponent(&velocitySlider, true);

    softButton.setEnabled(false);
    softButton.setBounds(180, 140, 250,20);
    addAndMakeVisible(softButton);

    messageLabel.setText("message", juce::dontSendNotification);
    messageLabel.setEnabled(false);
    messageLabel.setBounds(180, 180, 250, 20);
    addAndMakeVisible(messageLabel);

    ch1Slider.addListener(this);
    ch2Slider.addListener(this);
    velocitySlider.addListener(this);
}

SoftPedalAudioProcessorEditor::~SoftPedalAudioProcessorEditor()
{
}

//==============================================================================
void SoftPedalAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//  g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SoftPedalAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SoftPedalAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    *(audioProcessor.ch1) = (int)(ch1Slider.getValue());
    *(audioProcessor.ch2) = (int)(ch2Slider.getValue());
    *(audioProcessor.vel) = (int)(velocitySlider.getValue());
}

void SoftPedalAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.isSoftPedalOn)
        softButton.setToggleState(true, false);
    else
        softButton.setToggleState(false, false);

    messageLabel.setText("in["+ (juce::String)(audioProcessor.lastIn) + "] out["+ (juce::String)(audioProcessor.lastOut) +"]", juce::dontSendNotification);
}