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


    void updateCutFilter(std::atomic<float>* newFrequency) {

        //setHpfValue(newFrequency->load());
        currentHpfValue.setTargetValue(newFrequency->load());
        while (currentHpfValue.isSmoothing())
        {
            iirFilter.setCoefficients(juce::IIRCoefficients::makeHighPass
            (sampleRate, currentHpfValue.getNextValue()));
            //DBG(currentHpfValue.getNextValue());
        }
    }

    void reset() {
        iirFilter.reset();
        rmsLevelMid.reset(sampleRate, 0.5);
        rmsLevelSide.reset(sampleRate, 0.5);

        rmsLevelMid.setCurrentAndTargetValue(-100.f);
        rmsLevelSide.setCurrentAndTargetValue(-100.f);
        sumMid = 0.f,
            sumSide = 0.f;

        strVal.reset(sampleRate, 0.005);
        currentHpfValue.reset(sampleRate, 0.005);

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
    //void setHpfValue(float value) {


    //    if (value < currentHpfValue.getCurrentValue())
    //        currentHpfValue.setTargetValue(value);
    //    else
    //        currentHpfValue.setCurrentAndTargetValue(value);
    //}

private:


    float stereoWidth = 1.0f;

    double sampleRate = 48000;
    //float frequenzy = 20.f;
    juce::IIRFilter iirFilter;

    float sumMid = 0.f, sumSide = 0.f;
    juce::LinearSmoothedValue<float> rmsLevelMid, rmsLevelSide, strVal, currentHpfValue;

    bool midSolo = false, sideSolo = false;

    bool LRInputMode = true, LROutputMode = true;

    bool LRSwap = false;

    float M = 0.f;
    float S = 0.f;

    float L = 0.f;
    float R = 0.f;

    juce::LinearSmoothedValue<float> midGain = 0.0;
    juce::LinearSmoothedValue<float> sideGain = 0.0;
};