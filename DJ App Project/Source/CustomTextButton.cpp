#include <JuceHeader.h>
#include "CustomTextButton.h"


CustomTextButton::CustomTextButton(std::string _buttonName1, ButtonType _buttonType, DJAudioPlayer* _player)
: buttonName1(_buttonName1),
  buttonName2(""),
  buttonType(_buttonType),
  player(_player)
{
}

CustomTextButton::CustomTextButton(std::string _buttonName1, std::string _buttonName2, ButtonType _buttonType, DJAudioPlayer* _player)
: buttonName1(_buttonName1),
  buttonName2(_buttonName2),
  buttonType(_buttonType),
  player(_player)

{
}

CustomTextButton::~CustomTextButton()
{
    
}

void CustomTextButton::paint (juce::Graphics& g)
{
    //if it is a play button then draw the play's graphics.
    if(buttonType == ButtonType::play)
    {
        if(!clicked)
        {
            g.setColour(juce::Colours::white);
            g.drawText(buttonName1, getLocalBounds(), Justification::centred);
        }
        else
        {
            g.setColour(juce::Colours::green);
            g.drawText(buttonName2, getLocalBounds(), Justification::centred);
        }
        
        g.setFont(19);
        g.drawRoundedRectangle(0,0,getWidth(), getHeight(),18,5);
    }
    //if it is a stop button then draw the stop button's graphics.
    else if(buttonType == ButtonType::stop)
    {
        if(!clicked)
        {
            g.setColour(juce::Colours::white);
            g.drawText(buttonName1, getLocalBounds(), Justification::centred);
        }
        else
        {
            g.setColour(juce::Colours::green);
            g.drawText(buttonName1, getLocalBounds(), Justification::centred);
        }
        
        g.setFont(19);
        g.drawRoundedRectangle(0,0,getWidth(), getHeight(),18,5);
    }
    //if it is a Loop button then draw the Loop button's graphics.
    else if(buttonType == ButtonType::repeat)
    {
        if(!clicked)
        {
            g.setColour(juce::Colours::white);
            g.drawText(buttonName1, getLocalBounds(), Justification::centred);
        }
        else
        {
            g.setColour(juce::Colours::green);
            g.drawText(buttonName1, getLocalBounds(), Justification::centred);
        }
        
        g.setFont(19);
        g.drawRoundedRectangle(0,0,getWidth(), getHeight(),18,5);
    }
}
    

void CustomTextButton::resized()
{
}


void CustomTextButton::mouseDown(const juce::MouseEvent &event)
{

    //switch the clicked value
    clicked = !clicked;
    
    if(buttonType == ButtonType::play)
    {
        //functionality for play button
        if(clicked)
        {
            player->start();
        }else
        {
            player->stop();
        }
    }
    else if(buttonType == ButtonType::stop)
    {
        //functionality for stop button
        player->stop();
        player->setPosition(0);
        stopped = true;
    }
    else if(buttonType == ButtonType::repeat)
    {
        //functionality for repeat button
        if(clicked)
        {
            player->setLooping(true);
        }
        else
        {
            player->setLooping(false);
        }
    }
    
    //update paint function
    repaint();
}



//this function is mainly for the stop button, which immediately switches off when mouse click is lifted.
void CustomTextButton::mouseUp(const juce::MouseEvent &event)
{
    if(buttonType == ButtonType::stop)
    {
        //switch the clicked value
        clicked = !clicked;
        //update paint function
        repaint();
    }

}
