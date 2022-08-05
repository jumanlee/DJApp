#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
using namespace juce;

class SpeedButton  : public juce::Component
{
public:
    SpeedButton(DJAudioPlayer* _player);
    ~SpeedButton() override;
    
    //variable to count how many times the mouse is clicked in order to display the approrpriate speed measure text accordingly.
    int clickCount;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent &event) override;

private:
   
    DJAudioPlayer* player;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeedButton)
};
