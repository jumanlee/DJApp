
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

//add inheritance AudioSource into DJAudioPlayer
class DJAudioPlayer : public AudioSource
{
public:
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    double getPositionSeconds();
    double getLengthSeconds();
    
    void start();
    void stop();
    
    void setLooping(bool shouldLoop);
    
    /** returns the relative position of the playhead*/
    double getPositionRelative();
    
private:
    AudioFormatManager& formatManager; //changed to passing this as a reference cuz we want to pass tin reference and not make a copy of it
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    //false means don't mess with my pointer, 2 means 2 channels
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
};
