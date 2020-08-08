/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 22 Jul 2020 8:28:03pm
    Author:  Blair

  ==============================================================================
*/

#include "DJAudioPlayer.h"
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager
                            ) : formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    DBG("DJAudioPlayer::loadURL called");
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        
        ////juce::StringPairArray metadata = reader->metadataValues;
        juce::StringArray keys = reader->metadataValues.getAllKeys();
        int count = keys.size();
        DBG("The size of the metadata string array is " << count);
        DBG("The first key is " << keys[0]);
        for (juce::String key : reader->metadataValues.getAllKeys()) 
        {
            DBG("Key: " + key + " value: " + reader->metadataValues.getValue(key, "unknown"));
        }
    }
}
void DJAudioPlayer::play()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative position should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 10.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 10");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
