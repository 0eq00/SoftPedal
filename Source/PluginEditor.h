/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
/*
class SoftPedalAudioProcessorEditor  : public juce::AudioProcessorEditor
*/
class SoftPedalAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener, private juce::Timer
{
public:
    SoftPedalAudioProcessorEditor (SoftPedalAudioProcessor&);
    ~SoftPedalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoftPedalAudioProcessor& audioProcessor;

    juce::Slider velocitySlider;
    juce::Label velocityLabel;
    juce::ToggleButton softButton{ "Soft Pedal" };
    juce::Label messageLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftPedalAudioProcessorEditor)
};
