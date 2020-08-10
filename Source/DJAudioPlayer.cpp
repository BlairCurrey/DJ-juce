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
    //Default reverb settings
    juce::Reverb::Parameters params = reverbSource.getParameters();
    DBG("params room size: " << params.roomSize);
    DBG("params damping: " << params.damping);
    DBG("params wet level: " << params.wetLevel);
    DBG("params dry level: " << params.dryLevel);
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverbSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
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
        
        // Get metadata from files (Ogg Vorbis works best)
        //juce::StringArray keys = reader->metadataValues.getAllKeys();
        //int count = keys.size();
        //DBG("The size of the metadata string array is " << count);
        //DBG("The first key is " << keys[0]);
        //for (juce::String key : reader->metadataValues.getAllKeys()) 
        //{
        //    DBG("Key: " + key + " value: " + reader->metadataValues.getValue(key, "unknown"));
        //}
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
    if (ratio < 0.25 || ratio > 4.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0.25 and 4");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setRoomSize(double size)
{
    DBG("DJAudioPlayer::setRoomSize called");
    size = float(size);
    if (size < 0 || size > 1.0)
    {
        DBG("DJAudioPlayer::setRoomSize should be between");
    }
    else {
        reverbParameters.roomSize = size;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDamping(double dampingAmt)
{
    DBG("DJAudioPlayer::setDamping called");
    dampingAmt = float(dampingAmt);
    if (dampingAmt < 0 || dampingAmt > 1.0)
    {
        DBG("DJAudioPlayer::setDamping should be between");
    }
    else {
        reverbParameters.damping = dampingAmt;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setWetLevel(double wetLevel)
{
    DBG("DJAudioPlayer::setWetLevel called");
    wetLevel = float(wetLevel);
    if (wetLevel < 0 || wetLevel > 1.0)
    {
        DBG("DJAudioPlayer::setWetLevel should be between");
    }
    else {
        reverbParameters.wetLevel = wetLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDryLevel(double dryLevel)
{
    DBG("DJAudioPlayer::setDryLevel called");
    dryLevel = float(dryLevel);
    if (dryLevel < 0 || dryLevel > 1.0)
    {
        DBG("DJAudioPlayer::setDryLevel should be between");
    }
    else {
        reverbParameters.dryLevel = dryLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}
