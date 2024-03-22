/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftPedalAudioProcessor::SoftPedalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(ch1 = new juce::AudioParameterInt("ch1", "Channel1", 1, 16, 1 ));
    addParameter(ch2 = new juce::AudioParameterInt("ch2", "Channel2", 1, 16, 1 ));
    addParameter(vel = new juce::AudioParameterInt("vel", "Velocity", 0, 127, 8 ));

    isSoftPedalOn = false;
    lastIn = 1;
    lastOut = 1;
}

SoftPedalAudioProcessor::~SoftPedalAudioProcessor()
{
}

//==============================================================================
const juce::String SoftPedalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoftPedalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoftPedalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoftPedalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoftPedalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoftPedalAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoftPedalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoftPedalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SoftPedalAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoftPedalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SoftPedalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SoftPedalAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoftPedalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SoftPedalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    juce::MidiBuffer processedMidi;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isSoftPedalOn())
        {
            this->isSoftPedalOn = true;
        }

        if (message.isSoftPedalOff())
        {
            this->isSoftPedalOn = false;
        }

        if (message.isNoteOn())
        {
            this->lastIn = message.getVelocity();

            if (this->isSoftPedalOn)
            {
                this->lastOut = message.getVelocity() - (*(this->vel));
                if (this->lastOut < 1)
                    this->lastOut = 1;

                message = juce::MidiMessage::noteOn(*(this->ch2), message.getNoteNumber(), (juce::uint8)this->lastOut);
                processedMidi.addEvent(message, time);
            }
            else
            {
                this->lastOut = this->lastIn;

                message.setChannel(*(this->ch1));
                processedMidi.addEvent(message, time);
            }
        }
        else
        {
            if (message.getChannel() == 0)
            {
                processedMidi.addEvent(message, time);
            }
            else
            {
                message.setChannel(*(this->ch1));
                processedMidi.addEvent(message, time);
                if (this->ch1 != this->ch2)
                {
                    message.setChannel(*(this->ch2));
                    processedMidi.addEvent(message, time);
                }
            }
        }
    }

    midiMessages.swapWith(processedMidi);
/*
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
*/
}

//==============================================================================
bool SoftPedalAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SoftPedalAudioProcessor::createEditor()
{
    return new SoftPedalAudioProcessorEditor (*this);
}

//==============================================================================
void SoftPedalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("0eq00:SoftPedal"));
    xml->setAttribute("ch1", (int)(*ch1));
    xml->setAttribute("ch2", (int)(*ch2));
    xml->setAttribute("vel", (int)(*vel));
    copyXmlToBinary(*xml, destData);

    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SoftPedalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("0eq00:SoftPedal"))
        {
            *ch1 = xmlState->getIntAttribute("ch1");
            *ch2 = xmlState->getIntAttribute("ch2");
            *vel = xmlState->getIntAttribute("vel");
        }
    }

    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoftPedalAudioProcessor();
}
