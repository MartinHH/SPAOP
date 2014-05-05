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
    Component(componentName),
    x(0.5),
    y(0.5),
    sources_(nullptr),
    room_(nullptr),
    showOthers_(true),
    sourceID_(0),
    dotSize_(10),
    dotIsHit_(false)
{
}

SourcePanel::~SourcePanel()
{
}

void SourcePanel::paint (Graphics& g)
{
    
    g.fillAll (Colours::black);         // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    // draw Room if it is set:
    if (room_->getNumberOfVertices() > 1) {
        g.setColour(Colours::yellow);
        
        const int nVerts = room_->getNumberOfVertices();
        
        for(int i=0; i < nVerts; i++){
            wonder::Room::Vertex vert1 = room_->getVertex(i);
            wonder::Room::Vertex vert2 = room_->getVertex((i+1) % nVerts);
            
            const float xStart = COORD_NORM(vert1.x);
            const float yStart = COORD_NORM(vert1.y);
            const float xEnd = COORD_NORM(vert2.x);
            const float yEnd = COORD_NORM(vert2.y);
            g.drawLine(xStart * getWidth(),
                       yStart * getHeight(),
                       xEnd * getWidth(),
                       yEnd * getHeight(),
                       ROOM_LINE_THICKNESS);
        }
    }

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

void SourcePanel::setPositionDotSize(int diameter)
{
    if(dotSize_ != diameter){
        dotSize_ = diameter;
        repaint();
    }
}

int SourcePanel::getPositionDotSize()
{
    return dotSize_;
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

bool SourcePanel::setRoom(const wonder::Room *room)
{
    if (room_ == nullptr) {
        room_ = room;
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

bool SourcePanel::showsOtherSources()
{
    return showOthers_;
}
    
void SourcePanel::paintSource(Graphics& g, const wonder::Source& source, uint8_t alpha)
{
    
    const float x = source.getParameter(wonder::Source::xPosParam) * getWidth();
    const float y = source.getParameter(wonder::Source::yPosParam) * getHeight();
    
    g.setColour(Colour(source.getRed(), source.getGreen(),
                       source.getBlue(), alpha));
    
    if (source.getType() == wonder::Source::point) {
        // draw a circle:
        g.fillEllipse(x - dotSize_ / 2, y - dotSize_ / 2, dotSize_, dotSize_);
        
    } else {
        // draw an arrow:
        
        // for good visibility, the arrow needs to  be a bit bigger than the dot:
        const float size = dotSize_ * 1.25;
        
        // convert the angle to rad
        const float angleInRad = source.getAngle() * double_Pi / 180;
        
        // Coordinates of the arrow's tip:
        const float tipX = x + cos(angleInRad) * size;
        const float tipY = y + sin(angleInRad) * size;
        
        // Coordinates of the arrow's "base line":
        const float baseStartX = x + cos(angleInRad - float_Pi/2) * size/2;
        const float baseStartY = y + sin(angleInRad - float_Pi/2) * size/2;
        const float baseEndX = x + cos(angleInRad + float_Pi/2) * size/2;
        const float baseEndY = y + sin(angleInRad + float_Pi/2) * size/2;
        
        // Coordinates of arrow's tip "side lines":
        // To make the tip "pointy" the "side lines" need to overlap on the tip,
        // hence the individual start coordinates:
        
        const float sideThickness = size / 4;
        
        const float leftSideStartX = tipX - cos(angleInRad + float_Pi*3/4) * sideThickness/2;
        const float leftSideStartY = tipY - sin(angleInRad + float_Pi*3/4) * sideThickness/2;
        const float leftSideEndX = tipX + cos(angleInRad + float_Pi*3/4) * size/2;
        const float leftSideEndY = tipY + sin(angleInRad + float_Pi*3/4) * size/2;
        
        const float rightSideStartX = tipX - cos(angleInRad + float_Pi*5/4) * sideThickness/2;
        const float rightSideStartY = tipY - sin(angleInRad + float_Pi*5/4) * sideThickness/2;
        const float rightSideEndX = tipX + cos(angleInRad + float_Pi*5/4) * size/2;
        const float rightSideEndY = tipY + sin(angleInRad + float_Pi*5/4) * size/2;
        
        // draw those lines:
        g.drawLine(x, y, tipX, tipY, size/3);
        g.drawLine(baseStartX, baseStartY, baseEndX, baseEndY, size/3);
        g.drawLine(leftSideStartX, leftSideStartY, leftSideEndX, leftSideEndY, sideThickness);
        g.drawLine(rightSideStartX, rightSideStartY, rightSideEndX, rightSideEndY, sideThickness);
    }
}

}
