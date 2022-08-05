
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "CompLookAndFeel.h"
#include "SpeedButton.h"
#include "CustomTextButton.h"
#include "CueButton.h"
using namespace juce;


class MidPanel  : public juce::Component,
                  public juce::Button::Listener,
                  public juce::Slider::Listener
{
public:
    MidPanel(DJAudioPlayer* _player1, DJAudioPlayer* _player2);
    ~MidPanel() override;
    
    void buttonClicked(Button *) override;
    
    void sliderValueChanged(Slider *slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent &event) override;

private:
    
    Slider volSlider1;
    Slider volSlider2;
    Slider crossFaderSlider;
    
    Label sliderText;
    
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    
    CompLookAndFeel compLookAndFeel;
    
    //custom text buttons
    CustomTextButton playButton1;
    CustomTextButton playButton2;
    CustomTextButton stopButton1;
    CustomTextButton stopButton2;
    CustomTextButton repeatButton1;
    CustomTextButton repeatButton2;
    SpeedButton speedButton1;
    SpeedButton speedButton2;
    CueButton cue1L;
    CueButton cue2L;
    CueButton cue1R;
    CueButton cue2R;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidPanel)
};
