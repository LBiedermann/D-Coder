/*
  ==============================================================================

    MidSide.cpp
    Created: 24 Nov 2022 1:16:22pm
    Author:  Lukas

  ==============================================================================
*/

#include "MidSide.h"

void MidSide::midSideEncode() {
    M = 0.707f * (L + R);
    S = 0.707f * (L - R);
}

void MidSide::midSideDecode() {
    L = 0.707f * (M + S);
    R = 0.707f * (M - S);
}




void MidSide::processStereoWidth(juce::AudioBuffer<float>& buffer) {

    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    int N = buffer.getNumSamples();
    //processStereoWidth(leftChannel, rightChannel, N);

    sumMid = 0.f;
    sumSide = 0.f;

    for (int n = 0; n < N; n++)
    {
        
        //---------------------------------------------------
        //Input
        //---------------------------------------------------
        if (LRInputMode)
        {
            if (LRSwap && !LROutputMode)
            {
                R = leftChannel[n];
                L = rightChannel[n];
            }
            else
            {
                L = leftChannel[n];
                R = rightChannel[n];
            }

            // LR --> MS
            midSideEncode();
        }
        else 
        {
            M = leftChannel[n];
            S = rightChannel[n];
        }

        //---------------------------------------------------
        //SIDE Processing
        //---------------------------------------------------
        if (midSolo)
            S = 0.f;
        else {
            //EQ

            //iirFilter.processSamples(&S, 1);


            //store Sum for RMS
            sumSide += S * S;
        }


        //---------------------------------------------------
        //MID Processing
        //---------------------------------------------------
        if (sideSolo)
            M = 0.f;
        else {

            //Compressor

            //store Sum for RMS
            sumMid += M * M;
        }

        //---------------------------------------------------
        // Output
        //---------------------------------------------------
        if (LROutputMode)
        {
            // MS --> LR
            midSideDecode();
            if (LRSwap)
            {
                leftChannel[n] = R;
                rightChannel[n] = L;
            }
            else
            {
                leftChannel[n] = L;
                rightChannel[n] = R;
            }
        }
        else
        {
            leftChannel[n] = M;
            rightChannel[n] = S;
        }

    }

    //calcRMSLevel(N);
}

void MidSide::calcRMSLevel(int N) {

    rmsLevelMid.skip(N);
    rmsLevelSide.skip(N);
    {
        const auto midValue = juce::Decibels::gainToDecibels(std::sqrt(sumMid / N));
        if (midValue < rmsLevelMid.getCurrentValue())
            rmsLevelMid.setTargetValue(midValue);
        else
            rmsLevelMid.setCurrentAndTargetValue(midValue);
    }

    {
        const auto sideValue = juce::Decibels::gainToDecibels(std::sqrt(sumSide / N));
        if (sideValue < rmsLevelSide.getCurrentValue())
            rmsLevelSide.setTargetValue(sideValue);
        else
            rmsLevelSide.setCurrentAndTargetValue(sideValue);
    }

}
