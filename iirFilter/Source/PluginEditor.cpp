/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainTestProcessorEditor::GainTestProcessorEditor (GainTestProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    setSize(200, 200);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    filterCutoffDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setRange(20.0f, 20000.0f);
    filterCutoffDial.setValue(600.0f);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(filterCutoffDial);

    
    filterResDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterResDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    filterResDial.setRange(0.1f, 1.0f);
    filterResDial.setValue(2.0f);
    filterResDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(filterResDial);


    filterCutoffValue = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "cutoff", filterCutoffDial);
    filterResValue = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "resonance", filterResDial);
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f);

}

GainTestProcessorEditor::~GainTestProcessorEditor()
{
}

//==============================================================================
void GainTestProcessorEditor::paint (juce::Graphics& g)
{

    //background ui stuff

    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::steelblue);

    g.drawText("Filter", titleArea, juce::Justification::centredTop);
    g.drawText("Cutoff", 46, 70, 50, 25, juce::Justification::centredLeft);
    g.drawText("Resonance", 107, 70, 70, 25, juce::Justification::centredLeft);


    juce::Rectangle <float> area(25, 25, 150, 150);
    //g.setColour (juce::Colours::darkviolet);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
    g.setFont (15.0f);

}

void GainTestProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Rectangle<int> area = getLocalBounds().reduced(40);

    filterCutoffDial.setBounds(30, 90, 70, 70);
    filterResDial.setBounds(100, 90, 70, 70);
    
}
