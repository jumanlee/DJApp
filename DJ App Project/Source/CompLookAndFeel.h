/*
  ==============================================================================

    LookAndFeel.h
    Created: 18 Aug 2021 11:28:08am
    Author:  Jumanlee87

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

//==============================================================================
/*
*/
class CompLookAndFeel  : public juce::LookAndFeel_V4

{
public:
    CompLookAndFeel();
    ~CompLookAndFeel();
    
    void drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider &slider) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompLookAndFeel)
};
