/*
 * Copyright 2014 Martin Hansen
 *
 * This file is part of SPAOP (Spatial Audio Object Positioner).
 * 
 * SPAOP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SPAOP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SPAOP.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SourcePanel.h"
#include "JuceHeader.h"

namespace wonderjuce {

//==============================================================================
SourcePanel::SourcePanel(const String& componentName):
    SourceDisplay(componentName),
    x(0.5),
    y(0.5),
    sources_(nullptr),
    showOthers_(true),
    sourceID_(0),
    dotIsHit_(false)
{
}

SourcePanel::~SourcePanel()
{
}

void SourcePanel::paint (Graphics& g)
{
    
    // paint the room via the base class:
    SourceDisplay::paint(g);

    if(sources_ != nullptr){
        
        // paint the other sources (if requested):
        if (showOthers_) {
            for(int i=0; i < sources_->getMaxSources(); i++){
                if (i != sourceID_ ){
                    const wonder::Source& src = sources_->getSource(i);
                    if(src.isActive()){
                        paintSource(g, src, OTHER_SOURCES_ALPHA);
                    }
                }
            }
        }
    
        // paint this source
        const wonder::Source& source = sources_->getSource(sourceID_);
        paintSource(g, source, 0xFF);
        
        // update "local" coordinates to what has been painted:
        x = source.getParameter(wonder::Source::xPosParam);
        y = source.getParameter(wonder::Source::yPosParam);
    }
    

}

void SourcePanel::mouseDown(const MouseEvent &event)
{
    const wonder::Source& source_ = sources_->getSource(sourceID_);
    const float xDeNorm = source_.getParameter(wonder::Source::xPosParam) * getWidth();
    const float yDenorm = source_.getParameter(wonder::Source::yPosParam) * getHeight();
    
    // check whether the mouse click happened on the position dot:
    const float dotSize_ = getPositionDotSize();
    dotIsHit_ = (   event.getMouseDownX() >= xDeNorm - dotSize_/2
                 && event.getMouseDownX() <= xDeNorm + dotSize_/2
                 && event.getMouseDownY() >= yDenorm - dotSize_/2
                 && event.getMouseDownY() <= yDenorm + dotSize_/2 );
    
    if(dotIsHit_){
        // notify listeners the JUCE way:
        Component::BailOutChecker checker(this);
        listeners.callChecked(checker, &SourcePanel::Listener::sourcePanelDragStarted, this);
    }
}

void SourcePanel::mouseDrag(const MouseEvent &event)
{
    if (dotIsHit_) {        // only do something if the drag started on the dot...
        const Point<int> pos = event.getPosition();

        // calculate the relative postion:
        float xCoord = (float)pos.x / getWidth();
        float yCoord = (float)pos.y / getHeight();

        bool changed = (xCoord != x) || (yCoord != y);

        if (changed){
            x = xCoord;
            y = yCoord;

            // notify listeners the JUCE way:
            Component::BailOutChecker checker(this);
            listeners.callChecked(checker, &SourcePanel::Listener::sourcePanelValuesChanged, this);
            
            // request a repaint:
            repaint();
        }        
    }
}

void SourcePanel::mouseUp(const MouseEvent &event)
{
    if(dotIsHit_){          // only do something if the drag started on the dot...
        dotIsHit_ = false;  // reset that flag
        
        // notify listeners the JUCE way:
        Component::BailOutChecker checker(this);
        listeners.callChecked(checker, &SourcePanel::Listener::sourcePanelDragEnded, this);
    }
}

float SourcePanel::getXPos()
{
    return x;
}

float SourcePanel::getYPos()
{
    return y;
}

void SourcePanel::addListener(SourcePanel::Listener *listener)
{
    listeners.add(listener);
}

void SourcePanel::removeListener(SourcePanel::Listener *listener)
{
    listeners.remove(listener);
}
    
bool SourcePanel::setSources(std::shared_ptr<const wonder::SourceCollection> sources)
{
    if (sources_ == nullptr) {
        sources_ = sources;
        repaint();
        return true;
    } else {
        return false;
    }
}
    
bool SourcePanel::setSource(int sourceID)
{
    if(sources_ != nullptr && sourceID >=0 && sourceID < sources_->getMaxSources()){
        sourceID_ = sourceID;
        repaint();
        return true;
    } else {
        return false;
    }
}
    
void SourcePanel::setShowOtherSources(bool showOthers)
{
    showOthers_ = showOthers;
}

bool SourcePanel::showsOtherSources() const
{
    return showOthers_;
}

float SourcePanel::getFocusPointX()
{
    return x;
}
    
float SourcePanel::getFocusPointY()
{
    return y;
}
    
Point<float> SourcePanel::getFocusPoint()
{
    return Point<float>(x, y);
}

}
