/*
  ==============================================================================

    LargeKnob.h
    Created: 1 Feb 2023 3:09:47pm
    Author:  Lukas

  ==============================================================================
*/

#ifndef LARGEKNOB_H_INCLUDED
#define LARGEKNOB_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

class LargeKnob : public LookAndFeel_V4
{
public:
    Image sprite;
    double customSkew;


    void LargeKnob::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
    {
        if (sprite.isValid())
        {
            double rotation = (slider.getValue()
                - slider.getMinimum())
                / (slider.getMaximum()
                    - slider.getMinimum());
            rotation = std::pow(rotation, customSkew); //SKEW
            const int frames = 128;
            const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
            //const float radius = jmin(width / 1.0f, height / 1.0f);
            //const float centerX = x + width * 0.5f;
            //const float centerY = y + height * 0.5f;
            //const float rx = centerX - radius - 1.0f;
            //const float ry = centerY - radius;

            int imgWidth = sprite.getWidth();
            int imgHeight = sprite.getHeight() / frames;
            g.drawImage(sprite, 0, 0, imgWidth, imgHeight, 0, frameId * imgHeight, imgWidth, imgHeight);
        }
        else
        {
            static const float textPpercent = 0.35f;
            Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);

            g.setColour(Colours::white);

            g.drawFittedText(String("No Image"), text_bounds.getSmallestIntegerContainer(), Justification::horizontallyCentred | Justification::centred, 1);
        }
    }
};

#endif /* LARGEKNOB_H_INCLUDED */


