/*
  ==============================================================================

    VerticalGradientMeter.h
    Created: 24 Nov 2022 1:17:37pm
    Author:  Lukas

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
using namespace juce;
namespace Gui {

    class VerticalGradientMeter : public Component, public Timer {

    public:

        VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction)) {
            startTimer(30);
        }
        ~VerticalGradientMeter() {
            stopTimer();
        }

        void paint(Graphics& g) override {
            const auto level = valueSupplier();

            auto bounds = getLocalBounds().toFloat();
            g.setColour(Colours::black);
            g.fillRect(bounds);

            g.setGradientFill(gradient);
            const auto scaledY =
                jmap(level, -60.f, 0.f, 0.f, static_cast<float>(getHeight()));
            g.fillRect(bounds.removeFromBottom(scaledY));
        }

        void resized() override {
            const auto bounds = getLocalBounds().toFloat();
            gradient = ColourGradient{ Colours::green, bounds.getBottomLeft(), Colours::red, bounds.getTopLeft(), false };
            gradient.addColour(0.5, Colours::yellow);
        }

        void timerCallback() override {
            repaint();
        }

    private:
        std::function<float()> valueSupplier;
        ColourGradient gradient{};
    };
}
