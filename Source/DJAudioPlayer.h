/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 22 Jul 2020 8:28:03pm
    Author:  Blair

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        /**Loads the audio file*/
        void loadURL(juce::URL audioURL);
        /**Plays loaded audio file*/
        void play();
        /**Stops playing audio file*/
        void stop();
        /**Sets relative position of audio file*/
        void setPositionRelative(double pos);
        /**Sets the volume*/
        void setGain(double gain);
        /**Sets the speed*/
        void setSpeed(double ratio);
        /**Gets relative position of playhead*/
        double getPositionRelative();
        /**Gets the length of transport source in seconds*/
        double getLengthInSeconds();
        /**Sets the amount of reverb*/
        void setRoomSize(float size);
        /**Sets the amount of reverb*/
        void setDamping(float dampingAmt);
        /**Sets the amount of reverb*/
        void setWetLevel(float wetLevel);
        /**Sets the amount of reverb*/
        void setDryLevel(float dryLevel);
    private:
        void setPosition(double posInSecs);
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
        juce::ReverbAudioSource reverbSource{ &resampleSource, false };
        juce::Reverb::Parameters reverbParameters;
};