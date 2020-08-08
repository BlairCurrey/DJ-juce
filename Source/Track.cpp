/*
  ==============================================================================

    Track.cpp
    Created: 4 Aug 2020 10:16:10am
    Author:  Blair

  ==============================================================================
*/

#include "Track.h"
#include <filesystem>

Track::Track(juce::File _file) : file(_file), 
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
    DBG("Created new track with title: " << title);
    /*title = get title from filename
    artist = get artist from filename
    length = get length from filename*/


    //https://forum.juce.com/t/metadata-value-from-audio-files/19822/5
    //juce::AudioFormatManager formatManager;
    //formatManager.registerBasicFormats();

    //// in the loop:
    //juce::ScopedPointer<juce::AudioFormatReader> reader = formatManager.createReaderFor(boost::filesystem::directory_iterator.getFile());
    //if (reader) {
    //    for (juce::String key : reader->metadataValues.getAllKeys()) {
    //        DBG("Key: " + key + " value: " + reader->metadataValues.getValue(key, "unknown"));
    //    }
    //}
}

bool Track::operator==(const juce::String& other) const 
{
    return title == other;
}