/*
  ==============================================================================

    CoordinatePlot.h
    Created: 10 Aug 2020 5:23:41pm
    Author:  Blair

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CoordinatePlot  : public juce::Component
{
    public:
        CoordinatePlot();
        ~CoordinatePlot() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        //void setRange(double newMinimum, double newMaximum, double newInterval = 0);
    private:
        //uce::Range<double> range{0.0, 1.0};
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoordinatePlot)
};
