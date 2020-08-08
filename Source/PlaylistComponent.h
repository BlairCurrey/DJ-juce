/*
  ==============================================================================

    PlaylistComponent.h
    Created: 24 Jul 2020 3:29:26pm
    Author:  Blair

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include "Track.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, 
                      DeckGUI* _deckGUI2, 
                      DJAudioPlayer* _playerForParsingMetaData
                     );
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, 
                            int rowNumber, 
                            int width,
                            int height,
                            bool rowIsSelected
                           ) override;
    void paintCell(juce::Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                  ) override;
    
    Component* refreshComponentForCell(int rowNumber, 
                                       int columnId, 
                                       bool isRowSelected, 
                                       Component* existingComponentToUpdate) override;
    void buttonClicked(juce::Button* button) override;
    //void mouseDown(const juce::MouseEvent& event);
private:
    juce::TableListBox library;
    //std::vector<juce::String> tracks;
    std::vector<Track> tracks;
    juce::TextButton importButton{ "IMPORT TRACKS" };
    juce::TextButton addToPlayer1Button{ "ADD TO DECK 1" };
    juce::TextButton addToPlayer2Button{ "ADD TO DECK 2" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;
    
    void importToLibrary();
    void deleteFromTracks(int id);
    void loadInPlayer(DeckGUI* deckGUI);
    bool isInTracks(juce::String fileNameWithoutExtension);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
