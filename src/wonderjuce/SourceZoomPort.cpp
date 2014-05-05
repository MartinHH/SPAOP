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

#include "JuceHeader.h"
#include "SourceZoomPort.h"

namespace wonderjuce {

SourceZoomPort::SourceZoomPort(const String& componentName):
    Viewport(componentName),
    sourcePanel_(),
    zoom_(1.0)
{
    addAndMakeVisible(&sourcePanel_);
    setViewedComponent(&sourcePanel_);
}

SourceZoomPort::~SourceZoomPort()
{
}

void SourceZoomPort::paint (Graphics& g)
{
    Viewport::paint(g);
    sourcePanel_.paint(g);
}

void SourceZoomPort::resized()
{
    setSourcePanelSize();
}

void SourceZoomPort::setZoomFactor(float zoomFactor)
{
    if(zoomFactor != zoom_ && zoomFactor >= 1.0){
        zoom_ = zoomFactor;
        setSourcePanelSize();
        // set Focus on position-dot:
        setViewPositionProportionately(sourcePanel_.getXPos(), sourcePanel_.getYPos());
        if (zoom_ == 1.0){
            setViewPosition(0, 0); // makes sure the scrollbars disappear
        }
        repaint();
    }
}

float SourceZoomPort::getZoomFactor()
{
    return zoom_;
}

SourcePanel* SourceZoomPort::getSourcePanel()
{
    return &sourcePanel_;
}

void SourceZoomPort::setSourcePanelSize()
{
    sourcePanel_.setSize((int) getWidth() * zoom_,
                         (int) getHeight() * zoom_);
}

}
