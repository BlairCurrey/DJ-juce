/*
  ==============================================================================

    DeckGUI.h
    Created: 23 Jul 2020 12:16:53pm
    Author:  Blair

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
//#include "PlaylistComponent.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player, 
            juce::AudioFormatManager& formatManager, 
            juce::AudioThumbnailCache& thumbCache);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /**Implement Button::Listener*/
    void buttonClicked(juce::Button* button) override;
    /**Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;
    /**Detects if file is being dragged over deck*/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    /**Detects if file is dropped onto deck*/
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    /**Listen for changes to the waveform*/
    void timerCallback() override;

private:
    int id;
    
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::Slider volSlider;
    juce::Label volLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    juce::Slider posSlider;
    juce::Label posLabel;
    juce::Slider reverbSlider;
    juce::TextButton loadButton{ "LOAD" };

    void loadFile(juce::URL audioURL);

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    friend class PlaylistComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
