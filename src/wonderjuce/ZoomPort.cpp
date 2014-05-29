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

#include "ZoomPort.h"

namespace wonderjuce {
    
ZoomPort::ZoomPort(const String& componentName):
    Viewport(componentName),
    wrappedComponent_(nullptr),
    zoom_(1.0)
{
}
    
ZoomPort::~ZoomPort()
{
}
    
bool ZoomPort::setWrappedComponent(ComponentWithFocusPoint *wrappedComponent)
{
    std::lock_guard<std::mutex> lock(setMutex_);
    if(wrappedComponent_ == nullptr){
        wrappedComponent_ = wrappedComponent;
        addAndMakeVisible(wrappedComponent_);
        setViewedComponent(wrappedComponent_);
        return true;
    } else {
        return false;
    }
}
    
void ZoomPort::paint (Graphics& g)
{
    Viewport::paint(g);
}
    
void ZoomPort::resized()
{
    setWrappedComponentsSize();
}
    
void ZoomPort::setZoomFactor(float zoomFactor)
{
    if(zoomFactor != zoom_ && zoomFactor >= 1.0){
        zoom_ = zoomFactor;
        if(wrappedComponent_ != nullptr){
            setWrappedComponentsSize();
            // set Focus on position-dot:
            setViewPositionProportionately(wrappedComponent_->getFocusPointX(),
                                           wrappedComponent_->getFocusPointY());
            if (zoom_ == 1.0){
                setViewPosition(0, 0); // makes sure the scrollbars disappear
            }
            repaint();
        }
    }
}
    
float ZoomPort::getZoomFactor()
{
    return zoom_;
}
    
void ZoomPort::setWrappedComponentsSize()
{
    if(wrappedComponent_ != nullptr){
        wrappedComponent_->setSize((int) getWidth() * zoom_,
                                   (int) getHeight() * zoom_);
    }
}
    
}