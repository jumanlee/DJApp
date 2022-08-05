
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "SaveList.h"

using namespace juce;

class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::FileDragAndDropTarget,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _player);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    
    void paintRowBackground (Graphics &g,
                                int rowNumber,
                                int width,
                                int height,
                                bool rowIsSelected) override;
    
    void paintCell (Graphics &g,
                       int rowNumber,
                       int columnId,
                       int width,
                       int height,
                       bool rowIsSelected) override;
    
    Component * refreshComponentForCell (int rowNumber,
                                         int columnId,
                                         bool isRowSelected,
                                         Component * existingComponentToUpdate) override;
    
    void buttonClicked(Button* button) override;
    
    bool isInterestedInFileDrag(const StringArray &files) override;
    
    void filesDropped (const StringArray &files, int x, int y) override;
    
    void textEditorTextChanged (TextEditor &) override;
    
    void updateContent();

private:
    
    std::string lowerCase(std::string);
    
    std::string convertSeconds(double);
    
    TableListBox tableComponent;
    
    //Note about the 4 arrays below: I understand that it's entirely possible to only use two arrays rather than four. I can tehcnically just use one array to store the File type tracks and if I want to get the track titles, I can just use getFileNameWithoutExtension().toStdString() on each element whenever I need to access each of its string title; the second array works similarly and is used as the "Backup storage" for the search bar. However, it is in my opinion that having four clearly laid out arrays allows coders to visualise the workings more clearly, although care has to be taken in the sense that, when deleting the trackTitles array, the coder needs to remember to delete the trackFiles array at the same time, too. Another important factor is, this playlist contains placeholder texts at the start; having four arrays makes dealing with the situation where placeholder texts are present easier.
    
    //"original" arrays for storing track titles and File type tracks.
    std::vector<std::string> trackTitles;
    std::vector<File> trackFiles;
    
    //"backup storage" to store copies of track titles and File type tracks. This is mainly for the purpose of carrying out the song search (search bar) functionality.
    std::vector<std::string> store_trackTitles;
    std::vector<File> store_trackFiles;
    
    //connect the deck GUI
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    
    //this 3rd player is to help PlaylistComponent to manage and read audio files.
    DJAudioPlayer* player;
    
    //make a search bar for user to search songs
    TextEditor searchBar;
    
    //make a SaveList object to save the playlist in a csv file.
    SaveList saveList;
    
    //button for user to add songs to the music library.
    TextButton addButton{"Add to library"};
    
    std::string placeholderText = "Empty track: Drag files or use Add button to load songs";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
