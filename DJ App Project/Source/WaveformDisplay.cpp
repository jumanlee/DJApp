
#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse,
                                 AudioThumbnailCache & cacheToUse,
                                 DJAudioPlayer* _player)
                                :
                                audioThumb(1000, formatManagerToUse, cacheToUse), //note that audioThumb has already been declared in the header file. This is making it into an object using the 3 arguments
                                fileLoaded(false),
                                position(0),
                                player(_player)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
    
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll(Colour (0x194B0082));  // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::grey);
    if(fileLoaded)
    {
        //draw the thumbnail
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0, //left channel, only need to draw one.
                               1.0f);
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth()/20, getHeight()); //0 is y position.
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
}

//method to allow the waveform head to be repositioned (as well as the song position) when the user clicks on another location in the waveform component. 
void WaveformDisplay::mouseDown(const juce::MouseEvent &event)
{
    double mouseRelativeX = (double) event.getMouseDownX()/getWidth();
    
    setPositionRelative(mouseRelativeX);
}

void WaveformDisplay::loadURL(URL audioURL)
{
    //clear any possible old ones
    audioThumb.clear();
    
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL)); //unpack the url received and unpack it and turn it in to an input source. Also, setSource is a boolean type function (see documentation).
    
    if (fileLoaded)
    {
        repaint(); //i added this based on the Topic 10 code
    }
}


void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    repaint();
}

//remember, repaint() now includes the playhead
void WaveformDisplay::setPositionRelative(double pos)
{
    if(pos != position && pos == pos) //must add pos == pos otherwise wouldn't work! This is not mentioned in the lecture.
    {
        position = pos;
        player->setPositionRelative(pos);
        repaint();
    }
}

//public method to clear waveform
void WaveformDisplay::clearWaveform()
{
    //clear any possible old ones
    audioThumb.clear();
}

