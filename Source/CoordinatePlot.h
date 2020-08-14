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
class CoordinatePlot  : public juce::Component,
                        public juce::SettableTooltipClient
{
    public:
        CoordinatePlot();
        ~CoordinatePlot() override;

        void paint (juce::Graphics&) override;
        void resized() override;
        
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;

        /**Set the number of grid lines shown on the Coordinate Plot
        *  Defaults to 4 (4 vertical, 4 horizontal). Reduces lineCount
        *  by 1 if odd. Uses default if lineCount < 2*/
        void setGridLineCount(int lineCount=4);
        /**Set range*/
        void setRange(float min=0.0f, float max= 1.0f);
        /**Gets the current x coord*/
        float getX();
        /**Gets the current y coord*/
        float getY();

        class Listener
        {
            public:
                Listener();
                ~Listener();
                
                //listener callback
                virtual void coordPlotValueChanged(CoordinatePlot* coordinatePlot) = 0;
        };
        //calls listener callback
        void interactWithComponent();
        //add component to Listener list
        void addListener(Listener* l);
        //remove component from Listener list
        void removeListener(Listener* l);
    private:
        juce::ListenerList<Listener> listeners;

        std::map<char, float> coordsRaw;
        std::map<char, float> rangeRaw;
        void setRangeRaw();
        void initCoords(float rawX, float rawY);
        void setCoords(float rawX, float rawY);
        void updateCoords();

        //Positional Settings
        float midY;
        float midX;
        float left;
        float right;
        float top;
        float bottom;
        void setSettings();

        //User settings
        int gridLineCount;
        std::map<char, float> range;

        void drawPlot(juce::Graphics& g);
        void drawAxis(juce::Graphics& g);
        void drawGrid(juce::Graphics& g);
        void drawMarker(juce::Graphics& g);
        void drawText(juce::Graphics& g);
        bool markerMoved{ false };

        float constrain(float coord);
        float invertCoord(float coord, float min, float max);
        bool inRangeRaw(float rawX, float rawY);
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoordinatePlot)
};
