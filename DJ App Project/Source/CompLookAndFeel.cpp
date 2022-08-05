
#include <JuceHeader.h>
#include "CompLookAndFeel.h"
#include <algorithm>

CompLookAndFeel::CompLookAndFeel()
{
}

CompLookAndFeel::~CompLookAndFeel()
{
}

void CompLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider &slider)
{
    g.setColour(Colours::grey);
    
    //make thumb for the slider thumb
    Path thumb;
    
    if(slider.getSliderStyle() == Slider::SliderStyle::LinearVertical)
    {
        //draw the thumb of slider
        int thumbX = x;
        
        //loadfactor for deciding the size of the thumb size
        int loadFactor = 10;
        
        //thumb height pegged to load factor and slider height
        int thumbH = float(height/loadFactor);
        
        //maximum function to prevent the thumb from overshooting the slider when sliderPos gets to negative.
        int thumbY = sliderPos - thumbH/2;
        
        int thumbW = width;
        //set the graphics of the slider
//        g.drawRect(x,y - thumbH/2,width,height + thumbH);
        g.drawRoundedRectangle(x,y - thumbH/2,width,height + thumbH, 10,2);
        
        g.fillRect(x + width/4,y - thumbH/2,width - 2*(width/4),height + thumbH);
        
        g.setColour(Colours::white);
        
        //set the appearance of thumb
        thumb.addRoundedRectangle(thumbX, thumbY, thumbW, thumbH, 5);
        g.fillPath(thumb);
    }
    else if(slider.getSliderStyle() == Slider::SliderStyle::LinearHorizontal)
    {
        //loadfactor for deciding the size of the thumb size
        int loadFactor = 8;
        
        //draw the thumb of slider
        int thumbY = y;

        //thumb height pegged to load factor and slider height
        int thumbW = float(width/loadFactor);
        
        int thumbX = sliderPos - thumbW/2;
        
        int thumbH = height;
        
        g.drawRoundedRectangle(x - thumbW/2, y, width + thumbW, height, 10, 2);
        g.fillRect(x - thumbW/2, y + height/4, width + thumbW, height - 2*(height/4));

        g.setColour(Colours::white);
  
        //set the appearance of thumb
        thumb.addRoundedRectangle(thumbX, thumbY, thumbW, thumbH, 5);
        g.fillPath(thumb);
    }
}



