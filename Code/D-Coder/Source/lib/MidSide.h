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
    MidSide() {};
    void processStereoWidth(juce::AudioBuffer<float>& buffer);

    void midSideEncode();
    void midSideDecode();

    void setLCFilter(std::atomic<float>* newFreq) {
        currentLCValue.setTargetValue(newFreq->load());
    }

    void setHCFilter(std::atomic<float>* newFreq) {
        currentHCValue.setTargetValue(newFreq->load());
    }

    void setPFFreq(std::atomic<float>* newFreq) {
        currentPKFreqValue.setTargetValue(newFreq->load());
    }

    void setPFGain(std::atomic<float>* newGain) {
        currentPKGainValue.setTargetValue(newGain->load());
    }

    void setPFQuality(std::atomic<float>* newQ) {
        currentPKQualValue.setTargetValue(newQ->load());
    }

    void updateLowCutFilter() {
        lowCutFilterCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, currentLCValue.getNextValue());
    }

    void updateHighCutFilter() {
        highCutFilterCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, currentHCValue.getNextValue());
    }

    void updatePeakFilter() {
        peakFilterCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, currentPKFreqValue.getNextValue(), currentPKQualValue.getNextValue(), juce::Decibels::decibelsToGain(currentPKGainValue.getNextValue()));
    }

    void updateCompressor(float newThreshhold, float newRatio, float newAttack, float newRelease) {
        compressor.setThreshold(newThreshhold);
        compressor.setRatio(newRatio);
        compressor.setAttack(newAttack);
        compressor.setRelease(newRelease);
    }

    void reset() {

        rmsLevelMid.reset(sampleRate, 0.5);
        rmsLevelSide.reset(sampleRate, 0.5);

        rmsLevelMid.setCurrentAndTargetValue(-100.f);
        rmsLevelSide.setCurrentAndTargetValue(-100.f);
        sumMid = 0.f,
        sumSide = 0.f;

        midGain.reset(sampleRate, 0.0005);
        sideGain.reset(sampleRate, 0.0005);

        lowCutFilter.reset();
        highCutFilter.reset();
        peakFilter.reset();

        currentLCValue.reset(sampleRate, 0.05);
        currentHCValue.reset(sampleRate, 0.05);

        currentPKGainValue.reset(sampleRate, 0.05);
        currentPKFreqValue.reset(sampleRate, 0.05);
        currentPKQualValue.reset(sampleRate, 0.05);


        compressor.reset();
    }
    void setSamplerate(double newSamplerate) {
        sampleRate = newSamplerate;
    }
    void setSpec(juce::dsp::ProcessSpec newSpec) {
        compressor.prepare(newSpec);
        lowCutFilter.prepare(newSpec);
        highCutFilter.prepare(newSpec);
        peakFilter.prepare(newSpec);
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

    juce::dsp::IIR::Coefficients<float>::Ptr lowCutFilterCoeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr highCutFilterCoeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr peakFilterCoeffs;


    juce::dsp::Compressor<float> compressor;

    juce::LinearSmoothedValue<float> currentLCValue = 20.f;
    juce::LinearSmoothedValue<float> currentHCValue = 20000.f;

    juce::LinearSmoothedValue<float> currentPKGainValue = 0.f;
    juce::LinearSmoothedValue<float> currentPKFreqValue = 1000.f;
    juce::LinearSmoothedValue<float> currentPKQualValue = 0.5f;



};
