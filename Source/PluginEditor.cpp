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
    setSize(400, 300);
    startTimerHz(60);

    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setRange(0.0, 127.0, 1.0);
    velocitySlider.setBounds(100, 20, 250, 20);
    velocitySlider.setValue(audioProcessor.softVelocity);
    addAndMakeVisible(&velocitySlider);

    velocityLabel.setText("velocity", juce::dontSendNotification);
    velocityLabel.attachToComponent(&velocitySlider, true);

    softButton.setEnabled(false);
    softButton.setBounds(45, 60, 250,20);
    addAndMakeVisible(softButton);

    messageLabel.setText("message", juce::dontSendNotification);
    messageLabel.setEnabled(false);
    messageLabel.setBounds(45, 100, 250, 20);
    addAndMakeVisible(messageLabel);

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
    audioProcessor.softVelocity = (int)(velocitySlider.getValue());
}

void SoftPedalAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.isSoftPedalOn)
        softButton.setToggleState(true, false);
    else
        softButton.setToggleState(false, false);

    messageLabel.setText("in["+ (juce::String)(audioProcessor.lastIn) + "] out["+ (juce::String)(audioProcessor.lastOut) +"]", juce::dontSendNotification);
}