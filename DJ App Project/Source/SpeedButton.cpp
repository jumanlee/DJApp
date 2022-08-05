
#include <JuceHeader.h>
#include "SpeedButton.h"

//==============================================================================
SpeedButton::SpeedButton(DJAudioPlayer* _player)
: player(_player)
{
    
    //initialise the clickcount
    clickCount = 0;
}

SpeedButton::~SpeedButton()
{
}

void SpeedButton::paint (juce::Graphics& g)
{
    //logic for appearance of the speed button according to how many times the button has been clicked.
    if(clickCount == 0 || clickCount > 2)
    {
        g.setColour(juce::Colours::white);
        g.drawText("1x", 0, 0, getWidth(), getHeight(), Justification::centred);
        
    }
    else if(clickCount == 1)
    {
        g.setColour(juce::Colours::green);
        g.drawText("2x", 0, 0, getWidth(), getHeight(), Justification::centred);
        
    }
    else if(clickCount == 2)
    {
        g.setColour(juce::Colours::red);
        g.drawText("3x", 0, 0, getWidth(), getHeight(), Justification::centred);
    }
    
    //reset clickCount if it exceeds 2
    if(clickCount > 2)
    {
        clickCount = 0;
    }

    //draw the shape of the button
    g.drawEllipse((getWidth()*0.3)/2,(getHeight()*0.3)/2, getWidth()*0.7, getHeight()*0.7,3);
}

void SpeedButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void SpeedButton::mouseDown(const juce::MouseEvent &event)
{
    //update clickCount
    clickCount++;
    
    
    //the behaviour and action by the button is determined by how many times the button has been clicked: 
    if(clickCount > 2)
    {
        clickCount = 0;
    }
    
    if(clickCount == 1)
    {
        player->setSpeed(2);

    }else if(clickCount == 2)
    {
        player->setSpeed(3);
    }else
    {
        player->setSpeed(1);
    }
    
    //update the paint function
    repaint();
}



