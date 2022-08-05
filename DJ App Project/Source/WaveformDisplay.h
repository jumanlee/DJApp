
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
using namespace juce;

class WaveformDisplay  : public juce::Component,
                        public juce::ChangeListener //this is added to receive and listen to what is being broadcasted from audioThumb's broadcast.

{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse,
                    AudioThumbnailCache & cacheToUse,
                    DJAudioPlayer* _player);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void changeListenerCallback (ChangeBroadcaster *source) override; 
    
    //implement a listener for the broadcasting message from audioThumb. see documentation ChangeListener class or watch lesson 9.113
    void mouseDown(const juce::MouseEvent &event) override;
    
    void loadURL(URL audioURL);
    
    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);
    
    void clearWaveform();



private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    
    DJAudioPlayer* player;
    
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
