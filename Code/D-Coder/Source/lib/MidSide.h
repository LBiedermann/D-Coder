/*
  ==============================================================================

    MidSide.h
    Created: 24 Nov 2022 1:16:22pm
    Author:  Lukas

  ==============================================================================
*/

#include <JuceHeader.h>
//#include "EQ.h"


#pragma once

class MidSide {

public:

    void processStereoWidth(juce::AudioBuffer<float>& buffer);

    void midSideEncode();
    void midSideDecode();


    void updateLowCutFilter(float newFrequency) {
        lowCutFilter = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, newFrequency);
    }
    void updateHighCutFilter(float newFrequency) {
        highCutFilter = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, newFrequency);
    }
    void updatePeakFilter(float newFreq, float newGain, float newQ) {
        peakFilter = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, newFreq, newQ, juce::Decibels::decibelsToGain(newGain));
    }

    void reset() {

        rmsLevelMid.reset(sampleRate, 0.5);
        rmsLevelSide.reset(sampleRate, 0.5);

        rmsLevelMid.setCurrentAndTargetValue(-100.f);
        rmsLevelSide.setCurrentAndTargetValue(-100.f);
        sumMid = 0.f,
        sumSide = 0.f;


        midGain.reset(sampleRate, 0.00005);
        sideGain.reset(sampleRate, 0.00005);
    }
    void setSamplerate(double newSamplerate) {
        sampleRate = newSamplerate;
    }


    void calcRMSLevel(int N);


    float getRMSLevelMid() {
        return rmsLevelMid.getCurrentValue();
    }
    float getRMSLevelSide() {

        return rmsLevelSide.getCurrentValue();
    }

    void setMidState(bool midState) {
        midSolo = midState;
    }

    void setSideState(bool sideState) {
        sideSolo = sideState;
    }

    void setInputState(bool inState) {
        LRInputMode = inState;
    }

    void setOutputState(bool outState) {
        LROutputMode = outState;
    }
    void setSwapState(bool swapState) {
        LRSwap = swapState;
    }

    void setMidGain(float mGain) {
        midGain.setTargetValue(juce::Decibels::decibelsToGain(mGain));
    }
    void setSideGain(float sGain) {
        sideGain.setTargetValue(juce::Decibels::decibelsToGain(sGain));
    }


private:

    double sampleRate = 48000;

    float sumMid = 0.f, sumSide = 0.f;

    juce::LinearSmoothedValue<float> rmsLevelMid, rmsLevelSide;

    bool midSolo = false, sideSolo = false;

    bool LRInputMode = true, LROutputMode = true;

    bool LRSwap = false;

    float M = 0.f;
    float S = 0.f;

    float L = 0.f;
    float R = 0.f;

    juce::LinearSmoothedValue<float> midGain = 0.0;
    juce::LinearSmoothedValue<float> sideGain = 0.0;

    juce::dsp::IIR::Filter<float> highCutFilter, lowCutFilter, peakFilter;
};