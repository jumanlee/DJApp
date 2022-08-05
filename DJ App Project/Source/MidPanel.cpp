
#include <JuceHeader.h>
#include "MidPanel.h"
#include "DeckGUI.h"
#include <math.h>

//==============================================================================
MidPanel::MidPanel(DJAudioPlayer* _player1, DJAudioPlayer* _player2)
: player1(_player1),
  player2(_player2),
  speedButton1(_player1),
  speedButton2(_player2),
  playButton1("Play", "Pause", ButtonType::play, _player1),
  playButton2("Play", "Pause", ButtonType::play, _player2),
  stopButton1("Stop", ButtonType::stop, _player1),
  stopButton2("Stop", ButtonType::stop, _player2),
  repeatButton1("Loop", ButtonType::repeat, _player1),
  repeatButton2("Loop", ButtonType::repeat, _player2),
  cue1L("1", _player1),
  cue2L("2", _player1),
  cue1R("1", _player2),
  cue2R("2", _player2)

{
    addAndMakeVisible(speedButton1);
    addAndMakeVisible(speedButton2);
    addAndMakeVisible(playButton1);
    addAndMakeVisible(playButton2);
    addAndMakeVisible(stopButton1);
    addAndMakeVisible(stopButton2);
    addAndMakeVisible(repeatButton1);
    addAndMakeVisible(repeatButton2);
    addAndMakeVisible(cue1L);
    addAndMakeVisible(cue2L);
    addAndMakeVisible(cue1R);
    addAndMakeVisible(cue2R);
    
//use addMouseListener to listen to the child component. addListener is not applicable here as this is a custom button created by myself in a new class.
    stopButton1.addMouseListener(this, false);
    stopButton2.addMouseListener(this, false);

    //volume slider 1
    addAndMakeVisible(volSlider1);
    volSlider1.addListener(this);
    volSlider1.setRange(0.0, 1.0);
    volSlider1.setValue(0.5);
    
    //remove slider position text box
    volSlider1.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    //set slider style
    volSlider1.setSliderStyle(Slider::SliderStyle::LinearVertical);
    
    //volume slider 2
    addAndMakeVisible(volSlider2);
    volSlider2.addListener(this);
    volSlider2.setRange(0.0, 1.0);
    volSlider2.setValue(0.5);
    
    //remove slider position text box
    volSlider2.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    //set slider style
    volSlider2.setSliderStyle(Slider::SliderStyle::LinearVertical);
    
    //crossfader slider
    addAndMakeVisible(crossFaderSlider);
    crossFaderSlider.addListener(this);
    crossFaderSlider.setRange(0.0, 1.0);
    crossFaderSlider.setValue(0.5);
    //remove slider position text box
    crossFaderSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    //set slider style
    crossFaderSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

    //override and alter the juce graphics of the sliders.
    volSlider1.setLookAndFeel(&compLookAndFeel);
    
    volSlider2.setLookAndFeel(&compLookAndFeel);
    crossFaderSlider.setLookAndFeel(&compLookAndFeel);
}

MidPanel::~MidPanel()
{
    //reassign to nullpointer when destroying to prevent memory leak. This is necessary only for lookandfeel items, which are only sliders in this case.
    volSlider1.setLookAndFeel(nullptr);
    volSlider2.setLookAndFeel(nullptr);
    crossFaderSlider.setLookAndFeel(nullptr);
}

void MidPanel::paint (juce::Graphics& g)
{

    g.fillAll(Colour (0x194B0082));
    g.setColour (juce::Colours::grey);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    
    //draw all labels
   g.drawText("Volume L", getWidth()/9, getHeight()/45, 60, 20, Justification::centred, true);
    
    g.drawText("Volume R", getWidth() - getWidth()/9 - 60, getHeight()/45, 60, 20, Justification::centred, true);
    
    g.drawText("Hot Cues L", getWidth()/12, getHeight()/1.45, getWidth()/4, 20, Justification::centred, true);
    
    g.drawText("Hot Cues R", getWidth()*(7.0f/12.0f), getHeight()/1.45, getWidth()/4, 20, Justification::centred, true);
}

void MidPanel::resized()
{

    volSlider1.setBounds(getWidth()/9,30, getWidth()/12, getHeight()/2);
    
    volSlider2.setBounds(getWidth()- (getWidth()/9) - (getWidth()/12),30, getWidth()/12, getHeight()/2);
    
    crossFaderSlider.setBounds((getWidth() - getHeight()/2.5)/2 , getHeight()/1.65, getHeight()/2.5, getWidth()/12);
    
    playButton1.setBounds(getWidth()/4, getHeight()/8, getWidth()/5.5, getWidth()/10);
    playButton2.setBounds(getWidth()*(25.0f/44.0f), getHeight()/8, getWidth()/5.5, getWidth()/10);
    
    stopButton1.setBounds(getWidth()/4, getHeight()/4, getWidth()/5.5, getWidth()/10);
    stopButton2.setBounds(getWidth()*(25.0f/44.0f), getHeight()/4, getWidth()/5.5, getWidth()/10);
    
    speedButton1.setBounds(getWidth()/4,getHeight()/2.9,getWidth()/6,getWidth()/6);
    speedButton2.setBounds(getWidth()*(25.0f/44.0f),getHeight()/2.9,getWidth()/6,getWidth()/6);
    
    repeatButton1.setBounds(getWidth()/4,getHeight()/2.05,getWidth()/5.5,getWidth()/10);
    repeatButton2.setBounds(getWidth()*(25.0f/44.0f),getHeight()/2.05,getWidth()/5.5,getWidth()/10);
    
    //cue buttons
    cue1L.setBounds(getWidth()/12,getHeight()/1.3,getWidth()/6,getWidth()/6);
    cue2L.setBounds(getWidth()/4,getHeight()/1.3,getWidth()/6,getWidth()/6);
    cue1R.setBounds(getWidth()*(7.0f/12.0f),getHeight()/1.3,getWidth()/6,getWidth()/6);
    cue2R.setBounds(getWidth()*(9.0f/12.0f),getHeight()/1.3,getWidth()/6,getWidth()/6);
}

void MidPanel::buttonClicked(Button* button)
{
}

void MidPanel::sliderValueChanged (Slider *slider)
{
    if(slider == &volSlider1)
    {
        player1->setGain(slider->getValue());
    }
    
    if(slider == &volSlider2)
    {
        player2->setGain(slider->getValue());
    }
    
    if(slider == &crossFaderSlider)
    {
        //make two pointers that point at volSlider1 and volSlider2
        Slider* vol1 = &volSlider1;
        Slider* vol2 = &volSlider2;
                
        //using the pointer, adjust the thumb positions of volSlider1 and volSlider2 so that they follow the thumb position of crossFaderSlider.
        vol1->setValue(1 - slider->getValue());
        vol2->setValue(slider->getValue());
        
    }
}

void MidPanel::mouseDown(const juce::MouseEvent &event)
{
    //this is to reset the play button's display (if playbutton is on and is green and displays the word "pause", and if you press "stop", this resets the the play button display to its default mode). This piece of code can only be implemented here as only MidPanel has direct access to all the buttons. This is why the .clicked and .stopped properties in the customn button class have to be public. The same goes for the speedButton reset. HOWEVER, cue buttons will not be reset because we don't want the saved positions of tracks to be reset just because we have pressed "stop", we want the saved  cue positions to remain saved. The only way to reset the cue buttons is to hold the CTRL key and left-click on the relevant cue button. Repeat buttons are also not affected by the stop button, as I think it is up to the user to reset it individually, rather than letting the stop button reset it. 
    
    if(stopButton1.stopped)
    {
        //reset play button1
        playButton1.clicked = false;
        playButton1.repaint();
        stopButton1.stopped = false;
        
        //reset speed button1
        speedButton1.clickCount = 0;
        speedButton1.repaint();
        player1->setSpeed(1);
    }
    
    if(stopButton2.stopped)
    {
        //reset play button2
        playButton2.clicked = false;
        playButton2.repaint();
        stopButton2.stopped = false;
        
        //reset speed button2
        speedButton2.clickCount = 0;
        speedButton2.repaint();
        player2->setSpeed(1);
    }
}

