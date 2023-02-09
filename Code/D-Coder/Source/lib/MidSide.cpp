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
    S = S;
}

void MidSide::midSideDecode() {
    L = 0.707f * (M + S);
    R = 0.707f * (M - S);
}




void MidSide::processStereoWidth(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    auto leftChannel = block.getSingleChannelBlock(0);
    auto rightChannel = block.getSingleChannelBlock(1);
    int N = buffer.getNumSamples();

    //float* leftChannel = buffer.getWritePointer(0);
    //float* rightChannel = buffer.getWritePointer(1);
    //int N = buffer.getNumSamples();
    
    

    sumMid = 0.f;
    sumSide = 0.f;

    for (int n = 0; n < N; n++)
    {
        float gMid = midGain.getNextValue();
        float gSide = sideGain.getNextValue();

        //---------------------------------------------------
        //Input
        //---------------------------------------------------
        if (LRInputMode)
        {
            if (LRSwap && !LROutputMode)
            {
                R = leftChannel.getSample(0,n);
                L = rightChannel.getSample(0,n);
            }
            else
            {
                L = leftChannel.getSample(0, n);
                R = rightChannel.getSample(0, n);
            }

            // LR --> MS
            midSideEncode();
        }
        else
        {
            M = leftChannel.getSample(0, n);
            S = rightChannel.getSample(0, n);
        }



        //---------------------------------------------------
        //SIDE Processing
        //---------------------------------------------------
        if (midSolo && !sideSolo)
        {
            S = 0.f;
            //sideSolo = false; //muss im Editor noch angepasst werden
        }
        else {

            //EQ
//            if (currentPKFreqValue.isSmoothing() || currentPKGainValue.isSmoothing() || currentPKQualValue.isSmoothing()) {
            updatePeakFilter();
            //            }
            S = peakFilter.processSample(S);


            //Filter
            S = lowCutFilter.processSample(S);
            if (currentLCValue.isSmoothing()) {
                updateLowCutFilter();
                currentLCValue.skip(1);
            }
  
           

            //            if (currentHCValue.isSmoothing()) {
            updateHighCutFilter();
            //            }
            S = highCutFilter.processSample(S);

            //Gain
            S *= gSide;

            //store Sum for RMS
            sumSide += S * S;
        }

        //---------------------------------------------------
        //MID Processing
        //---------------------------------------------------
        if (sideSolo && !midSolo)
        {
            M = 0.f;
            //midSolo = false;
        }
        else {
            //Compressor
            if (!isnan(M) && !isinf(M)) {
                M = compressor.processSample(0, M);//std::abs(M));

            }


            //Gain
            M *= gMid;

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
                leftChannel.setSample(0, n, R);
                rightChannel.setSample(0, n, L);
            }
            else
            {
                leftChannel.setSample(0, n, L);
                rightChannel.setSample(0, n, R);
            }
        }
        else
        {
            leftChannel.setSample(0, n, M);
            rightChannel.setSample(0, n, S);
        }

        lowCutFilter.snapToZero();
        highCutFilter.snapToZero();
        peakFilter.snapToZero();
    }


    calcRMSLevel(N);
}

void MidSide::calcRMSLevel(int N) {

    rmsLevelMid.skip(N);
    rmsLevelSide.skip(N);
    {
        const auto midValue = juce::Decibels::gainToDecibels(std::sqrt(sumMid / N));
        //if (midValue < rmsLevelMid.getCurrentValue())
            rmsLevelMid.setTargetValue(midValue);
        //else
        //    rmsLevelMid.setCurrentAndTargetValue(midValue);
    }

    {
        const auto sideValue = juce::Decibels::gainToDecibels(std::sqrt(sumSide / N));
        //if (sideValue < rmsLevelSide.getCurrentValue())
            rmsLevelSide.setTargetValue(sideValue);
        //else
        //    rmsLevelSide.setCurrentAndTargetValue(sideValue);
    }

}
