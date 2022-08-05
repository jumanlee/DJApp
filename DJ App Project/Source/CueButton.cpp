

#include <JuceHeader.h>
#include "CueButton.h"

//==============================================================================
CueButton::CueButton(std::string _buttonName, DJAudioPlayer* _player)
: buttonName(_buttonName),
    player(_player)
{

}

CueButton::~CueButton()
{
}

void CueButton::paint (juce::Graphics& g)
{
    if(!buttonOn)
    {
        g.setColour(juce::Colours::white);
        g.drawText(buttonName, getLocalBounds(), Justification::centred);
    }
    else
    {
        g.setColour(juce::Colours::green);
        g.drawText("Saved", getLocalBounds(), Justification::centred);
    }
    
    g.setFont(19);
    g.drawRoundedRectangle(0,0,getWidth(), getHeight(),18,5);
}

void CueButton::mouseDown(const juce::MouseEvent &event)
{
    //ModifierKeys is a static function. This condition is met if cue button is clicked while holding control key. This is used to reset the hot cue button.
    
    //when playCueCall is true, it means the saved cue position is being used to set the player position. When saveCueCall is true, it means there the current position will be saved by the cue button. if buttonOn is true, it means the button is currently green and says "saved" on the label. If ModifierKeys::currentModifiers.isCtrlDown() is true, it means the CTRL is being held down.
    if(ModifierKeys::currentModifiers.isCtrlDown())
    {
        playCueCall  = false;
        saveCueCall  = false;
        buttonOn = false;
    }else if(buttonOn == false)
    {
        playCueCall  = false;
        saveCueCall  = true;
        buttonOn = true;
    }else if(buttonOn == true)
    {
        playCueCall  = true;
        saveCueCall  = false;
        buttonOn = true;
    }
    
   if(saveCueCall)
   {
       saveCue(player->getPositionSeconds());
   }
   else if(playCueCall)
   {
       player->setPosition(getCue());
   }

    repaint();
}

void CueButton::resized()
{

}

void CueButton::saveCue(double _cueValue)
{
    cueValue = _cueValue;
}

double CueButton::getCue()
{
    return cueValue;
}

