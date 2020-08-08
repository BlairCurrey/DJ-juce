/*
  ==============================================================================

    Track.h
    Created: 4 Aug 2020 10:16:10am
    Author:  Blair

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Track
{
    public:
        Track(juce::File _file);
        juce::URL URL;
        juce::String title;
        juce::String artist;
        juce::String length;
        /**objects are compared by title*/
        bool operator==(const juce::String& other) const;
    private:
        juce::File file;
};