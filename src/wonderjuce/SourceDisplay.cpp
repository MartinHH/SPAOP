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

#include "SourceDisplay.h"

namespace wonderjuce {

//==============================================================================
SourceDisplay::SourceDisplay(const String& componentName,
                             int dotSize, int roomLineThickness,
                             const Colour roomColour,
                             const Colour bgColour):
    ComponentWithFocusPoint(componentName),
    room_(nullptr),
    showNames_(false),
    dotSize_(dotSize),
    roomLineThickness_(roomLineThickness),
    roomColour_(roomColour),
    bgColour_(bgColour)
{
}

SourceDisplay::~SourceDisplay()
{
}
    
void SourceDisplay::paint (Graphics& g)
{
        
    g.fillAll (bgColour_);         // clear the background
    
        
    // draw Room if it is set:
    if (room_ != nullptr && room_->getNumberOfVertices() > 1) {
        g.setColour(roomColour_);
            
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
                       roomLineThickness_);
            }
        }
}

void SourceDisplay::setPositionDotSize(int diameter)
{
    if(dotSize_ != diameter){
        dotSize_ = diameter;
        repaint();
    }
}

int SourceDisplay::getPositionDotSize() const
{
    return dotSize_;
}
    
bool SourceDisplay::setRoom(std::shared_ptr <const wonder::Room> room)
{
    if (room_ == nullptr) {
        room_ = room;
        repaint();
        return true;
    } else {
        return false;
    }
}
    
void SourceDisplay::setShowNames(bool showNames)
{
    showNames_ = showNames;
}
    
bool SourceDisplay::showsNames() const
{
    return showNames_;
}

void SourceDisplay::paintSource(Graphics& g, const wonder::Source& source, uint8_t alpha)
{
    
    const float x = source.getParameter(wonder::Source::xPosParam) * getWidth();
    const float y = source.getParameter(wonder::Source::yPosParam) * getHeight();
    
    g.setColour(Colour(source.getRed(), source.getGreen(),
                       source.getBlue(), alpha));
    
    if (source.getType() == wonder::Source::point) {
        // draw a circle:
        g.fillEllipse(x - dotSize_ / 2, y - dotSize_ / 2, dotSize_, dotSize_);
        
    } else {
        // This draws an arrow - the arrow's base will be positioned at the source's
        // position (this piece of code might need some refactoring, but basically
        // this is just a few rotations of certain points that are used to draw
        // lines):
        
        // for good visibility, the arrow needs to  be a bit bigger than the dot:
        const float size = dotSize_ * 1.25;
        
        // convert the angle to rad
        const float angleInRad = source.getAngle() * double_Pi / 180;
        
        // Coordinates of the arrow's tip - the tip is size pixels away from the source's
        // position:
        const float tipX = x + cos(angleInRad) * size;
        const float tipY = y + sin(angleInRad) * size;
        
        // Coordinates of the arrow's "base line" - it is drawn size pixels long
        // (size/2 pixels in each direction) and verical to the source's angle:
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
    
    if(showNames_){
        const float fontSize = g.getCurrentFont().getHeight();
        g.drawSingleLineText(source.getName(), x + 1.5 * dotSize_, y + fontSize/2 - 1);
    }
    
}

}