
#include <JuceHeader.h>
#include "DeckGUI.h"
#include <cmath>

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager & formatManagerToUse,
                 AudioThumbnailCache & cacheToUse
                 ) : player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse, player)
{
    addAndMakeVisible(waveformDisplay);
    
    //add itself as a listener
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);
    
    startTimer(500); //500milliseconds call everytime
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll(Colour (0x194B0082));  // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    
    g.setOrigin(0,0);
    g.setColour(juce::Colours::black);

    float wheelSize = (float) getWidth()*0.6;
    float wheelX = (float) getWidth()/2;
    float wheelY = (float) (getHeight() - getHeight()*((float)3/8))/(float)2;
    
    g.setOrigin(-wheelSize/2,-wheelSize/2);
    g.fillEllipse(wheelX, wheelY, wheelSize, wheelSize);
    
    g.setOrigin(wheelSize/2,wheelSize/2);
    g.setColour(juce::Colours::white);
    juce::Line<float> line (juce::Point<float> (wheelX, wheelY),
                                juce::Point<float> (wheelX , wheelY - (wheelSize/2.0f)));
    
    g.addTransform(AffineTransform::rotation(angle, wheelX, wheelY));

    g.drawLine (line, 5.0f);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    waveformDisplay.setBounds(0, rowH*5, getWidth(), rowH*3);
}

void DeckGUI::buttonClicked(Button* button)
{
//    //if the memory address stored in button is the same as the memory address stored in playButton, then execute:
    if(button == &playButton)
    {
        player->start();
    }
    if(button == &stopButton)
    {
        player->stop();
    }
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if(chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()}); //note waveformDisplay is not a pointer.
        }
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if(slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if(slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if(slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    if(player->getPositionRelative() >= 1)
    {
        waveformDisplay.setPositionRelative(0);
    }
    else
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}

void DeckGUI::loadFile(URL url)
{
    player->loadURL(url);
    waveformDisplay.loadURL(url); //note waveformDisplay is not a pointer.
}

void DeckGUI::mouseDrag(const MouseEvent &event)
{
    float wheelX = (float) getWidth()/2;
    float wheelY = (float) (getHeight() - getHeight()*((float)3/8))/(float)2;
    
    juce::Point<float> point = juce::Point<float> (wheelX, wheelY);
    
    double angleGain = point.getAngleToPoint(juce::Point<float> (event.x, event.y));
    
    //the following ensures that the angle returned is not suddenly negative the moment it crosses the 180 degree mark. The built-in juce functionality outputs a negative angle (radian) value when it is within the range of 180 deg to 360 deg. To mitigate this, the following is to ensure that all the radian values are positive.
    if(angleGain < 0)
    {
        angle = 2*M_PI + angleGain;
    }else
    {
        angle = angleGain;
    }
    
    if(((double) angle/(2*M_PI)) >0)
    {
        player->setSpeed(((double) angle/(2*M_PI)));
    }
    
    repaint();
}



