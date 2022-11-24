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

    void processStereoWidth(float& L, float& R);
    void processStereoWidth(float* leftChannel, float* rightChannel, const int N);
    void processStereoWidth(juce::AudioBuffer<float>& buffer);

    void midSideEncode(const float& L, const float& R, float& M, float& S);
    void midSideDecode(const float& M, const float& S, float& L, float& R);

    //void setStereowidthValue(std::atomic<float>* newWidth) {
    //    strVal.setTargetValue(newWidth->load());
    //    while (strVal.isSmoothing())
    //    {
    //        stereoWidth = strVal.getNextValue();
    //        //DBG(stereoWidth);
    //    }

    //}

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
        midMute = midState;
    }

    void setSideState(bool sideState) {
        sideMute = sideState;
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

    bool midMute = false, sideMute = false;
};