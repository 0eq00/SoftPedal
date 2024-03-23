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

    ch1Label.setText("Channel for Soft Pedal OFF", juce::dontSendNotification);
    ch1Label.setBounds(10, 20, 250, 20);
    addAndMakeVisible(&ch1Label);

    ch1Slider.setSliderStyle(juce::Slider::LinearHorizontal);
    ch1Slider.setRange(1, 16, 1);
    ch1Slider.setBounds(220, 20, 250, 20);
    ch1Slider.setValue(*(audioProcessor.ch1));
    addAndMakeVisible(&ch1Slider);

    ch2Label.setText("Channel for Soft Pedal ON", juce::dontSendNotification);
    ch2Label.setBounds(10, 60, 250, 20);
    addAndMakeVisible(&ch2Label);

    ch2Slider.setSliderStyle(juce::Slider::LinearHorizontal);
    ch2Slider.setRange(1, 16, 1);
    ch2Slider.setBounds(220, 60, 250, 20);
    ch2Slider.setValue(*(audioProcessor.ch2));
    addAndMakeVisible(&ch2Slider);

    velLabel.setText("Decrement amount of Velocity", juce::dontSendNotification);
    velLabel.setBounds(10, 100, 250, 20);
    addAndMakeVisible(&velLabel);

    velSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velSlider.setRange(0, 127, 1);
    velSlider.setBounds(220, 100, 250, 20);
    velSlider.setValue(*(audioProcessor.vel));
    addAndMakeVisible(&velSlider);

    softButton.setEnabled(false);
    softButton.setBounds(10, 140, 250,20);
    addAndMakeVisible(softButton);

    msgLabel.setText("message", juce::dontSendNotification);
    msgLabel.setEnabled(false);
    msgLabel.setBounds(220, 140, 250, 20);
    addAndMakeVisible(msgLabel);

    ch1Slider.addListener(this);
    ch2Slider.addListener(this);
    velSlider.addListener(this);
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
    *(audioProcessor.vel) = (int)(velSlider.getValue());
}

void SoftPedalAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.isSoftPedalOn)
        softButton.setToggleState(true, false);
    else
        softButton.setToggleState(false, false);

    msgLabel.setText("in["+ (juce::String)(audioProcessor.lastIn) + "] out["+ (juce::String)(audioProcessor.lastOut) +"]", juce::dontSendNotification);
}