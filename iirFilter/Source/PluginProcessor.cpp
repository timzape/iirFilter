/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainTestProcessor::GainTestProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), tree(*this, nullptr), lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1))
#endif
{
    juce::NormalisableRange<float> cutoffRange(20.0f, 20000.0f);
    juce::NormalisableRange<float> resRange(0.1f, 1.0f);


    tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 100.0f, nullptr, nullptr);
    tree.createAndAddParameter("resonance", "Resonance", "resonance", resRange, 0.1f, nullptr, nullptr);

    tree.state = juce::ValueTree();

}

GainTestProcessor::~GainTestProcessor()
{
}

//==============================================================================
const juce::String GainTestProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainTestProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainTestProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainTestProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainTestProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainTestProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainTestProcessor::getCurrentProgram()
{
    return 0;
}

void GainTestProcessor::setCurrentProgram (int index)
{
}

const juce::String GainTestProcessor::getProgramName (int index)
{
    return {};
}

void GainTestProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainTestProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    lowPassFilter.prepare(spec);
    lowPassFilter.reset();

}

void GainTestProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainTestProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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


void GainTestProcessor::updateFilter()
{
    float freq = *tree.getRawParameterValue("cutoff");
    float res = *tree.getRawParameterValue("resonance");
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freq, res);
}


void GainTestProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    updateFilter();
    lowPassFilter.process(juce::dsp::ProcessContextReplacing<float> (block));


}



//==============================================================================
bool GainTestProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainTestProcessor::createEditor()
{
    return new GainTestProcessorEditor (*this);
}

//==============================================================================
void GainTestProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GainTestProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainTestProcessor();
}
