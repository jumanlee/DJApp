#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <string.h>
#include <cctype>
#include <algorithm>
#include <math.h>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _player)
: deckGUI1(_deckGUI1),
  deckGUI2(_deckGUI2),
  player(_player)
{
    //reset both arrays before pushing back for defensive coding
    trackFiles.clear();
    trackTitles.clear();
    
    //if there is nothing saved in textFile.csv, then just display the placeholder text
    if(saveList.isEmpty())
    {
        for(int i = 0; i < 9; i++)
        {
            trackTitles.push_back(placeholderText);
        }
    }
    else
    {
        //if there is something saved in textFile.csv, then pushed all those csv lines into the four arrays (which include the store backup arrays for the search music functionality)
        std::vector<File> savedList = saveList.load();

        for(int i = 0; i < savedList.size(); i++)
        {
            trackFiles.push_back(savedList[i]);
            store_trackFiles.push_back(savedList[i]);
            trackTitles.push_back(savedList[i].getFileNameWithoutExtension().toStdString());
            store_trackTitles.push_back(savedList[i].getFileNameWithoutExtension().toStdString());
        }
    }
    
    //build all the necessary components for the table
    tableComponent.getHeader().addColumn("Track title", 1, 300);
    tableComponent.getHeader().addColumn("Track length", 2, 100);
    tableComponent.getHeader().addColumn("File size", 3, 100);
    tableComponent.getHeader().addColumn("Load L", 4, 10);
    tableComponent.getHeader().addColumn("Load R", 5, 10);
    tableComponent.getHeader().addColumn("Remove", 6, 10);
    tableComponent.getHeader().setStretchToFitActive(true);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBar);
    addAndMakeVisible(addButton);
    searchBar.addListener(this);
    addButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    //before the app is destructed, we need to first clear the saveList (which saves the playlist songs) and then save the latest trackFiles into a csv.
    saveList.clearList();
    saveList.save(trackFiles);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    
    g.setFont (20.0f);
//    g.drawText ("Drag files here to load songs", getLocalBounds(),
//                juce::Justification::centred, true);
    
    g.drawText("Search music: ", 0, 0, getWidth()*0.2, getHeight()*0.1, Justification::centred);
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0,0.11*getHeight(),getWidth(), getHeight());
    searchBar.setBounds(getWidth()*0.2,0,getWidth()*0.8, getHeight()*0.1);
    addButton.setBounds(getWidth()*0.8, 0, getWidth()*0.2, getHeight()*0.1);
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

//So the graphics context is going to represent just the bit of the row that I'm allowed to draw in. So that's commonly how it works. Think of how all the other components are only allowed to draw into the space that you've given them. So in this case, we've been given just a row to draw into. So we say, if rowIsSelected, I can do g.fillAll, and then I need to set it a color.
void PlaylistComponent::paintRowBackground (Graphics &g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected)
{
    if(!rowIsSelected)
    {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell (Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected)
{
    if(rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    
    if(columnId == 1)
    {
        g.drawText(trackTitles[rowNumber],
                   2, 0,
                   width - 4,
                   height,
                   Justification::centredLeft,
                   true);
    }
    
    if(trackTitles[0] != placeholderText)
    {
        if(columnId == 2)
        {
            player->loadURL(URL{File{trackFiles[rowNumber]}});
            
            double songLengthSeconds = player->getLengthSeconds();
            
            g.drawText(convertSeconds(songLengthSeconds),
                       2, 0,
                       width - 4,
                       height,
                       Justification::centredLeft,
                       true);
        }
        
        if(columnId == 3)
        {
            //get size in int64 data type.
            int64 fileSize = trackFiles[rowNumber].getSize();
            
            //then convert it to juce String and then to std::string
            String description_size = File::descriptionOfSizeInBytes(fileSize);
            std::string string_size = description_size.toStdString();
            
            g.drawText(string_size,
                       2, 0,
                       width - 4,
                       height,
                       Justification::centredLeft,
                       true);
        }
    }
}

Component * PlaylistComponent::refreshComponentForCell (
                                    int rowNumber,
                                     int columnId,
                                     bool isRowSelected,
                                     Component * existingComponentToUpdate)
{
    if(columnId == 4)
    {
        //if we haven't created a component yet, then..
        if(existingComponentToUpdate == nullptr)
        {
            TextButton*  btn = new TextButton{"left"};
            
            //below is a juce string. So first of all, we want an ID, which is of the type JUCE string, and I've got to pass something into the constructor. So what am I going to pass in? Well, I'm going to say standard to string. I'm going to pass it the row number. So this in a code, I'm passing to the constructor is taking an integer and converting into a standard string, and then the JUCE string is happy to receive the standard string into its constructor
            String id{std::to_string(rowNumber)};
            
            btn->setComponentID(id);
            
            String stringColumnId{std::to_string(columnId)};
            
            btn->setName(stringColumnId);
            
            btn->addListener(this);
            
            //remember textbutton is a child class of Component, that's why it's allowed to store a textbutton here:
            existingComponentToUpdate = btn;
        }
    }
    
    if(columnId == 5)
    {
        //if we haven't created a component yet, then..
        if(existingComponentToUpdate == nullptr)
        {
           TextButton* btn = new TextButton{"right"};
            
            //below is a juce string. So first of all, we want an ID, which is of the type JUCE string, and I've got to pass something into the constructor. So what am I going to pass in? Well, I'm going to say standard to string. I'm going to pass it the row number. So this in a code, I'm passing to the constructor is taking an integer and converting into a standard string, and then the JUCE string is happy to receive the standard string into its constructor
            String id{std::to_string(rowNumber)};
            
            btn->setComponentID(id);
            
            String stringColumnId{std::to_string(columnId)};
            
            btn->setName(stringColumnId);
            
            btn->addListener(this);
            
            //remember textbutton is a child class of Component, that's why it's allowed to store a textbutton here:
            existingComponentToUpdate = btn;
        }
    }
    
    if(columnId == 6)
    {
        //if we haven't created a component yet, then..
        if(existingComponentToUpdate == nullptr)
        {
            TextButton*  btn = new TextButton{"X"};
            
            //below is a juce string. So first of all, we want an ID, which is of the type JUCE string, and I've got to pass something into the constructor. So what am I going to pass in? Well, I'm going to say standard to string. I'm going to pass it the row number. So this in a code, I'm passing to the constructor is taking an integer and converting into a standard string, and then the JUCE string is happy to receive the standard string into its constructor
            String id{std::to_string(rowNumber)};
            
            btn->setComponentID(id);
            
            String stringColumnId{std::to_string(columnId)};
            
            btn->setName(stringColumnId);
            
            btn->addListener(this);
            
            //remember textbutton is a child class of Component, that's why it's allowed to store a textbutton here:
            existingComponentToUpdate = btn;
        }
    }
    
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    
    //this functionality is only executed if the button pressed is an addButton.
    if (button == &addButton)
    {
        //open up a pop up window for user to choose file
        FileChooser chooser{"Select a file..."};
        if(chooser.browseForFileToOpen())
        {
            //if when the user wants to add a new song, the placeholder text must be deleted first.
            if(trackTitles[0] == placeholderText)
            {
                trackTitles.clear();
                trackFiles.clear();
            }

            trackFiles.push_back(chooser.getResult());
            trackTitles.push_back(chooser.getResult().getFileNameWithoutExtension().toStdString());
            
            //once a new song is added, the table must be updated to reflect the change.
            tableComponent.updateContent();
        }
    }
    //the below is only executed if the button pressed is not an addButton. This safeguard is necessary to prevent error, especially because we are using button->getComponentID() below.
    else
    {
        // it starts off with a JUCE string, converts into a standard string, and then converts that into an integer.
        int rowId = std::stoi(button->getComponentID().toStdString());
        
        int columnId = std::stoi(button->getName().toStdString());

        if(columnId == 4 && trackTitles[0] != placeholderText)
        {
            deckGUI1->loadFile(URL{File{trackFiles[rowId]}});
        }
        else if(columnId == 5 && trackTitles[0] != placeholderText)
        {
            deckGUI2->loadFile(URL{File{trackFiles[rowId]}});
        }
        else if(columnId == 6)
        {
            //this below if condition is necessary, because if the app is first started and the playlist is filled with placeholder texts, the trackFiles array is empty, only the trackTitles array contains elements. To prevent the .erase method from trying to delete an empty array and cause problems, this if condition acts as a safeguard.
            if(trackTitles[0] == placeholderText)
            {
                //only delete the placeholder text
                trackTitles.erase(trackTitles.begin() + rowId);
            }
            else
            {
                //search the "store" arrays for the track chosen to be deleted and delete it there. This uses the simple linear search algorithm.
                for(int i = 0; i < store_trackTitles.size(); i++)
                {
                    if(store_trackTitles[i] == trackTitles[rowId])
                    {
                        store_trackTitles.erase(store_trackTitles.begin() + i);
                        store_trackFiles.erase(store_trackFiles.begin() + i);
                    }
                }
                
                //then remove the chosen track in the trackTitles and trackFiles arrays
                trackTitles.erase(trackTitles.begin() + rowId);
                trackFiles.erase(trackFiles.begin() + rowId);
            }
            
            //once removed, table needs to be updated to be redrawn
            tableComponent.updateContent();
        }
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray &files)
{
    return true;
}

void PlaylistComponent::filesDropped (const StringArray &files, int x, int y)
{
    if(files.size() > 0 && trackTitles[0] == placeholderText)
    {
        trackTitles.clear();
        trackFiles.clear();
    }

    for(int i = 0; i < files.size(); i++)
    {

        std::string title = File{files[i]}.getFileNameWithoutExtension().toStdString();
        //inner for-loop to check for duplicate - whether the song to be added is already on the playlist. If so, then don't add the duplicate.
        
        bool duplicate = false;
        
        for(int j = 0; j < trackTitles.size(); j++)
        {
            if(title == trackTitles[j])
            {
                duplicate = true;
                break;
            }
        }

        if(duplicate == false)
        {
            trackFiles.push_back(files[i]);
            trackTitles.push_back(title);
        }
    }
    
    tableComponent.updateContent();
    
    //the following section is for copying and saving all the tracks that have just been dropped into two "backup storage" arrays, one for the std::string titles and the other for File types. These two "backup storage" arrays are used for the search playlist functionality (please see textEditorTextChanged method below for more details)
    
    //reset all store arrays
    store_trackTitles.clear();
    store_trackFiles.clear();
    
    //save all the original information into two "backup storage" arrays
    for(int i = 0; i < trackTitles.size(); i++)
    {
        store_trackTitles.push_back(trackTitles[i]);
        store_trackFiles.push_back(trackFiles[i]);
    }
}

void PlaylistComponent::textEditorTextChanged (TextEditor &)
{

    //this if condition is necessary, because if the app is first started and the playlist is filled with placeholder texts. We don't want the texteditortextchanged functionality to function when there are placeholder texts in place. This if condition acts as a safeguard.
    if(trackTitles[0] != placeholderText)
    {
        //make a variable to store the word/title being typed into the search bar
        std::string searched_string = searchBar.getText().toStdString();
        
        //if the text editor text is not empty..
        if(!searched_string.empty())
        {
            //delete all elements in the original trackTitles and trackFiles so that the new elements can replace them
            trackTitles.clear();
            trackFiles.clear();
            
            //search the "backup storage" titles array (store_trackTitles) for the keyword being search for by the user (searched_string)
            for(int i = 0; i < store_trackTitles.size(); i++)
            {
                //convert both the store_trackTitles and searched_string elements into lower case and compare them so that the search bar is case insensitive.
                if(lowerCase(store_trackTitles[i]).find(lowerCase(searched_string)) != std::string::npos)
                {
                    trackTitles.push_back(store_trackTitles[i]);
                    trackFiles.push_back(store_trackFiles[i]);
                }
            }
        }
        else
        {
            //reset the trackTitles and trackFiles
            trackTitles.clear();
            trackFiles.clear();
            
            //if the searched_string from text editor is empty, then push all elements of the "store" arrays (containing the original elements stored when the text editor was first activated) into trackTitles and trackFiles. This is because when the search bar doesn't contain any text (no search being carried out by user) we want the playlist to display all the tracks that were initially dropped.
            for(int i = 0; i < store_trackTitles.size(); i++)
            {
                trackTitles.push_back(store_trackTitles[i]);
                trackFiles.push_back(store_trackFiles[i]);
            }
        }
        

        //update the table with the new changes
        tableComponent.updateContent();
    }
}

//this method is to convert all characters in a string to all lower case. I don't feel like using the built-in juce method for converting to lowercase because the juce method only works with juce strings. It gets messy if I try to convert std::string to juce string just to convert it to lower case then change it back to std::string. I prefer to construct my own method that works with std::string to carry out this task.
std::string PlaylistComponent::lowerCase(std::string word)
{
    std::string lowercase_word = word;
    std::for_each(lowercase_word.begin(), lowercase_word.end(), [](char& c)
    {
        c = ::tolower(c);
    });

      return lowercase_word;
}
    
//method to convert seconds (double) into std::string
std::string PlaylistComponent::convertSeconds(double seconds)
{
    int hours, minutes;
    int int_seconds = (int)seconds;
    minutes = seconds / 60;
    hours = minutes / 60;
    std::string string_holder;
    
    
    if(floor(hours) == 0)
    {
        string_holder = std::to_string((int) floor(minutes%60)) + ":" + std::to_string((int)floor(int_seconds%60));
    }
    else if(floor(hours) > 0)
    {
        string_holder = std::to_string((int)floor(hours)) + ":" + std::to_string((int)floor(minutes%60)) + ":" + std::to_string((int)floor(int_seconds%60));
    }
    
    return string_holder;
}


