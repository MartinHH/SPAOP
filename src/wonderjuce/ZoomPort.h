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

#ifndef ZOOMPORT_H_INCLUDED
#define ZOOMPORT_H_INCLUDED

#include <mutex>
#include "JuceHeader.h"
#include "ComponentWithFocusPoint.h"

namespace wonderjuce
{

//==============================================================================
/**
 *  A GUI component that wraps another ComponentWithFocusPoint, adding the
 *  ability to zoom in and out and to scroll.
 *
 *  Note that the child component is assumed to have the same size as this
 *  component: setting the zoom-factor to 1.0 will set the size of the wrapped
 *  component to the size of this component.
 */
class ZoomPort    : public Viewport
{
public:
    /** Constructor. */
    ZoomPort(const String& componentName = String::empty);
    
    /** Destructor. */
    virtual ~ZoomPort();
    
    /** Sets the component that is wrapped by this ZoomPort. This may only be
     *  done once.
     *
     *  @param wrappedComponent The component to be wrapped in this ZoomPort.
     *
     *  @return true on success, false if a wrapped component is already set.
     */
    bool setWrappedComponent(ComponentWithFocusPoint* wrappedComponent);
    
    void paint (Graphics&);
    void resized();
    
    /** Sets the zoom-factor. A zoom-factor of 1.0 will set the size of the
     *  wrapped component to the size of this ZoomPort. A zoom-factor x > 1.0
     *  will set the size of the wrapped component to x * the size of this
     *  ZoomPort.
     *
     *  @param zoomFactor A zoom-factor >= 1.0.
     */
    void setZoomFactor(float zoomFactor);
    
    /** Returns the current zoom-factor.
     *
     *  @return The current zoom-factor.
     *
     *  @see setZoomFactor
     */
    float getZoomFactor();
    
private:
    void setWrappedComponentsSize();
    
    ComponentWithFocusPoint* wrappedComponent_;
    float zoom_;            // zoom-factor
    std::mutex setMutex_;   // a mutex that guards setWrappedComponent
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZoomPort)
};

}


#endif  // ZOOMPORT_H_INCLUDED
