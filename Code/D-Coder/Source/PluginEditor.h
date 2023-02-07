/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "lib/LargeKnob.h"
#include "lib/VerticalGradientMeter.h"
using namespace juce;
//==============================================================================
/**
*/
class DCoderAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DCoderAudioProcessorEditor (DCoderAudioProcessor&);
    ~DCoderAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void paintOverChildren(juce::Graphics&) override;
    void resized() override;
    void addRotaries();
    void addButtons();


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DCoderAudioProcessor& audioProcessor;


    Image bgImage, grill, midSoloImage, midSoloLightImage, sideSoloImage, sideSoloLightImage, inImage, inLightImage, outImage, outLightImage, swapImage, swapLightImage;
    Image midLableImage, sideLableImage, lineLableImage, generalLableImage;


    LargeKnob ratioKnob, attackKnob, releaseKnob, thresholdKnob, midGainKnob;
    std::unique_ptr<Slider> ratioSlider, attackSlider, releaseSlider, thresholdSlider, midGainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  ratioAttachment, attackAttachment, releaseAttachment, thresholdAttachment, midGainAttachment;

    LargeKnob lcKnob, hcKnob, mGKnob, mFreqKnob, mQKnob, sideGainKnob;
    std::unique_ptr<Slider> lcSlider, hcSlider, mGSlider, mFreqSlider, mQSlider, sideGainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  lcAttachment, hcAttachment, mGAttachment, mFreqAttachment, mQAttachment, sideGainAttachment;

    Gui::VerticalGradientMeter verticalGradientMeterM, verticalGradientMeterS;

    ImageButton midSoloBtn, sideSoloBtn, inBtn, outBtn, swapBtn;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> midSoloAttachment, sideSoloAttachment, inAttachment, outAttachment, swapAttachment;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DCoderAudioProcessorEditor)
};
