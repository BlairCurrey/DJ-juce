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
    reverbParameters.roomSize = 0;
    reverbParameters.damping = 0;
    reverbParameters.wetLevel = 0;
    reverbParameters.dryLevel = 1.0;
    reverbSource.setParameters(reverbParameters);
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

void DJAudioPlayer::setRoomSize(float size)
{
    DBG("DJAudioPlayer::setRoomSize called");
    if (size < 0 || size > 1.0)
    {
        DBG("DJAudioPlayer::setRoomSize size should be between 0 and 1.0");
    }
    else {
        reverbParameters.roomSize = size;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDamping(float dampingAmt)
{
    DBG("DJAudioPlayer::setDamping called");
    if (dampingAmt < 0 || dampingAmt > 1.0)
    {
        DBG("DJAudioPlayer::setDamping amount should be between 0 and 1.0");
    }
    else {
        reverbParameters.damping = dampingAmt;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setWetLevel(float wetLevel)
{
    DBG("DJAudioPlayer::setWetLevel called");
    if (wetLevel < 0 || wetLevel > 1.0)
    {
        DBG("DJAudioPlayer::setWetLevel level should be between 0 and 1.0");
    }
    else {
        reverbParameters.wetLevel = wetLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDryLevel(float dryLevel)
{
    DBG("DJAudioPlayer::setDryLevel called");
    if (dryLevel < 0 || dryLevel > 1.0)
    {
        DBG("DJAudioPlayer::setDryLevel level should be between 0 and 1.0");
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
