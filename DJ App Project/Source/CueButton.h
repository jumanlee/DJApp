
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
using namespace juce;


class CueButton  : public juce::Component
{
    public:
        CueButton(std::string _buttonName, DJAudioPlayer* _player);
        ~CueButton() override;

        void mouseDown(const juce::MouseEvent &event) override;
        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        void saveCue(double _cueValue);
        double getCue();
        
        //boolean to record the state of the button
        bool saveCueCall = false;
        bool playCueCall = false;
        bool buttonOn = false;
        std::string buttonName;
        double cueValue = 0;
        
        DJAudioPlayer* player;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CueButton)
};
