/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 24 Jul 2020 3:29:26pm
    Author:  Blair

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, 
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData
                                    ) : deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2),
                                        playerForParsingMetaData(_playerForParsingMetaData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    library.getHeader().addColumn("Tracks", 1, 235);
    library.getHeader().addColumn("", 2, 30);
    //library.getHeader().addColumn("", 3, 50);
    library.setModel(this);
    
    addAndMakeVisible(importButton);
    addAndMakeVisible(library);
    addAndMakeVisible(addToPlayer1Button);
    addAndMakeVisible(addToPlayer2Button);

    importButton.addListener(this);
    addToPlayer1Button.addListener(this);
    addToPlayer2Button.addListener(this);
    library.addMouseListener(this, true);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    //                   x start, y start, width, height
    importButton.setBounds(0, 0, getWidth(), getHeight() / 16);
    library.setBounds(0, getHeight() / 16, getWidth(), 14 * getHeight() / 16);
    addToPlayer1Button.setBounds(0, 15 * getHeight() / 16, getWidth() / 2, getHeight() / 16);
    addToPlayer2Button.setBounds(getWidth() / 2, 15 * getHeight() / 16, getWidth() / 2, getHeight() / 16);
}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected
                                          )
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected
                                 )
{
    if (rowNumber < getNumRows())
    {
        g.drawText(tracks[rowNumber].title,
            2,
            0,
            width - 4,
            height,
            juce::Justification::centredLeft,
            true
        );
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "X" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    //if (columnId == 3)
    //{
    //    if (existingComponentToUpdate == nullptr)
    //    {
    //        juce::TextButton* btn = new juce::TextButton{ "X" };
    //        juce::String id{ std::to_string(rowNumber) };
    //        btn->setComponentID(id);

    //        btn->addListener(this);
    //        existingComponentToUpdate = btn;
    //    }
    //}
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importButton)
    {
        DBG("Load button clicked");
        importToLibrary();
        library.updateContent();
    }
    else if (button == &addToPlayer1Button)
    {
        DBG("Add to Player 1 clicked");
        loadInPlayer(deckGUI1);
    }
    else if (button == &addToPlayer2Button)
    {
        DBG("Add to Player 2 clicked");
        loadInPlayer(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(tracks[id].title + " removed from Library");
        deleteFromTracks(id);
        library.updateContent();
    }
}

//void PlaylistComponent::mouseDown(const juce::MouseEvent& event)
//{
//    //get selected row number
//    int selectedRow{ library.getSelectedRow() };
//    int rowHeight = library.getRowHeight();
//    int mouseX = event.getMouseDownX();
//    int mouseY = event.getMouseDownY() + rowHeight * selectedRow + 1;
//
//    DBG("mouseX " << mouseX);
//    DBG("mouseY " << mouseY);
//
//    //if mouse over selected row
//    juce::Rectangle<int> selectedRowCoords = library.getCellPosition(1,selectedRow - 1,true);
//
//    DBG("rowX " << selectedRowCoords.getX());
//    DBG("rowY " << selectedRowCoords.getY());
//    // deselect
//    //if (selectedRowCoords.contains(event.getPosition()))
//    bool isMouseOver = selectedRowCoords.contains(mouseX, mouseY);
//
//    if (isMouseOver && selectedRow >= 0)
//    {
//        DBG("Clicked on track: " << tracks[selectedRow]);
//        library.flipRowSelection(selectedRow);
//    }
//}

void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ library.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << tracks[selectedRow].title << " to Player 1");
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Information:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }
}

void PlaylistComponent::importToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    //initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if not already loaded
            {
                Track newTrack{ file };
                DBG("loaded file: " << newTrack.title);
                tracks.push_back(newTrack);
            }
            else // display info message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "Load information:",
                    fileNameWithoutExtension + " already loaded",
                    "OK",
                    nullptr
                );
            }
        }
    }
}

bool PlaylistComponent::isInTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}

void PlaylistComponent::deleteFromTracks(int id)
{
    tracks.erase(tracks.begin() + id);
}

//void PlaylistComponent::findIDFromCoords(int rowNumber, int colNumber)
//{
//  https://stackoverflow.com/questions/16819113/mapping-unique-points-to-unique-integer-index
//}
//
//void PlaylistComponent::findCoordsFromID(int rowNumber, int colNumber)
//{
//
//}