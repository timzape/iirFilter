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
class GainTestProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GainTestProcessorEditor (GainTestProcessor&);
    ~GainTestProcessorEditor();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::Slider filterCutoffDial;

    juce::Slider filterResDial;
    
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> filterResValue;

    GainTestProcessor& processor;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainTestProcessorEditor)
};
