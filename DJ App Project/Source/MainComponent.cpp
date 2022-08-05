#include "MainComponent.h"
#include <string>


//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open. This first value has been changed from 2 to 0 by MYK. See 7.403 14.11 minute
        setAudioChannels (0, 2);
    }
    
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(midPanel);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(searchBar);

    //prepares the formatManager so that it's ready to read some audio files. Otherwise it wouldn't know anything about any audio format.
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //player1 is DJ audio player variable
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    //false because we don't need it to delete the source once it's done cuz we are managing it on our own.
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //what's going to happen there is the MixerAudioSource is going to dig into all the sources that you've added to it. So the two players in. It'll ask them for their blocks, mix them together, and then give you back the resulting block.
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
   // For more details, see the help for AudioProcessor::releaseResources()
//    transportSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0,0,getWidth()/3, getHeight()*0.55);
    deckGUI2.setBounds(getWidth()*0.6667, 0, getWidth()/3, getHeight()*0.55);
    midPanel.setBounds(getWidth()/3,0, getWidth()/3 ,getHeight()*0.55);
    
    playlistComponent.setBounds(0, getHeight()*0.55, getWidth(), getHeight()*0.45);
}
