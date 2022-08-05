#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
using namespace juce;

//enum class to allow the user to determine which type of button to be created. There are only three types of buttons allowed in this class:
enum class ButtonType{play, stop, repeat};

class CustomTextButton  : public juce::Component
{
public:
    CustomTextButton(std::string _buttonName1, ButtonType _buttonType, DJAudioPlayer* _player);
    CustomTextButton(std::string _buttonName1, std::string _buttonName2, ButtonType _buttonType, DJAudioPlayer* _player);
    
    ~CustomTextButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;

    //boolean to record the state of a stop buttontype
    bool stopped = false;
    
    //boolean to record the state of the button
    bool clicked = false;

private:
    
    std::string buttonName1;
    std::string buttonName2;
    
    //boolean variable to check if this button is intended to be transitioned form one text to the other eg: from "Play" to "Pause" and vice versa. If yes, then this is true.

    DJAudioPlayer* player;
    
    ButtonType buttonType;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomTextButton)
};
