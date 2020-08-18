/*
  ==============================================================================

    WaveformDisplay.h
    Created: 24 Jul 2020 10:55:44am
    Author:  Blair

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(int _id,
                    juce::AudioFormatManager& formatManager,
                    juce::AudioThumbnailCache& thumbCache);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void loadURL(juce::URL audioURL);
    /**set the relative position of the playhead*/
    void setPositionRelative(double pos);
private:
    int id;
    bool fileLoaded;
    double position;
    juce::String fileName;
    juce::AudioThumbnail audioThumb;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
