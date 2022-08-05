#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
using namespace juce;

class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* _player,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse);

    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button *) override;
    
    void sliderValueChanged(Slider *slider) override;

    bool isInterestedInFileDrag(const StringArray &files) override;
    
    void filesDropped (const StringArray &files, int x, int y) override;
    
    void timerCallback() override;
    
    void mouseDrag(const MouseEvent &event) override;
    
    void loadFile(URL url);

private:
    
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
    
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    
    DJAudioPlayer* player;
    
    WaveformDisplay waveformDisplay;
    
    double angle = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
