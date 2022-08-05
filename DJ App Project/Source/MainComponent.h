#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "MidPanel.h"
#include "PlaylistComponent.h"
using namespace juce;

class MainComponent  : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:

    //for waveform thumbnail
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; //will store up to 100 cached waveforms any one time.
    
    
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};
    
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};
    
    //3rd player created mainly to help PlaylistComponent object read audio files (in order to display text such as audio length). 
    DJAudioPlayer player3{formatManager};
    
    MixerAudioSource mixerSource;
    
    PlaylistComponent playlistComponent{&deckGUI1, &deckGUI2, &player3};
    
    MidPanel midPanel{&player1, &player2};
    
    TextEditor searchBar;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
