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

#ifndef COMPONENTWITHFOCUSPOINT_H_INCLUDED
#define COMPONENTWITHFOCUSPOINT_H_INCLUDED

#include "JuceHeader.h"

namespace wonderjuce {

/**
 *  An abstract class extending juce::Component, adding methods to
 *  query a "focus point". The focus point is the point of the component
 *  where a ZoomPort shall zoom to when it zooms in.
 */
class ComponentWithFocusPoint   :   public Component
{
public:
    /** Contructor. */
    ComponentWithFocusPoint(const String& componentName = String::empty):
        Component(componentName)
    {}
    
    /** Destructor. */
    virtual ~ComponentWithFocusPoint(){}
    
    /** Returns the "focus point", the point where this component shall
     *  be auto-centered (for example, when it is zoomed into).
     *
     *  @return The "focus point", the point where this component shall
     *      be auto-centered (for example, when it is zoomed into). It
     *      contains relative coordinates within [0.0 ; 1.0].
     */
    virtual Point<float> getFocusPoint() = 0;
    
    /** Returns the x-coordinate of the "focus point".
     *
     *  @return The x-coordinate of the "focus point" as relative coordinate
     *      within [0.0 ; 1.0].
     */
    virtual float getFocusPointX() = 0;
    
    /** Returns the y-coordinate of the "focus point".
     *
     *  @return The y-coordinate of the "focus point" as relative coordinate
     *      within [0.0 ; 1.0].
     */
    virtual float getFocusPointY() = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentWithFocusPoint)
};
    
}

#endif  // COMPONENTWITHFOCUSPOINT_H_INCLUDED
