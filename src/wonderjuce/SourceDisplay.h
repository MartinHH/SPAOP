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

#ifndef SOURCEDISPLAY_H_INCLUDED
#define SOURCEDISPLAY_H_INCLUDED

#include <memory>

#include "Room.h"
#include "SourceCollection.h"
#include "ComponentWithFocusPoint.h"
#include "JuceHeader.h"

namespace wonderjuce {

/**
 *  An abstract class for a GUI component that displays a two-dimensional view of
 *  WONDER sources and the speaker arrays.
 *  The methods of ComponentWithFocusPoint, desired reactions to mouse-actions
 *  must be implemented in a derived class. Also, this class does not actually
 *  paint any sources, it only offers a paintSource method that must be called
 *  from derived classes.
 */
class SourceDisplay    : public ComponentWithFocusPoint
{
public:
    /** Constructor.
     *
     *  @param componentName The name of this component.
     *  @param dotSize The size of the dot indicating the source's position
     *      (in pixels).
     *  @param roomLineThickness The thickness of the line that indicates the
     *      speaker arrays (in pixels).
     *  @param roomColour The colour of the speaker arrays.
     *  @param bgColour The background colour.
     */
    SourceDisplay(const String& componentName = String::empty,
                  int dotSize = 10, int roomLineThickness = 2,
                  const Colour roomColour = Colours::yellow,
                  const Colour bgColour = Colours::black);
    
    /** Destructor. */
    virtual ~SourceDisplay();
    
    /** Overriding juce::Component::paint, this will fill the display with its
     *  background colour and draw the speaker arrays. In derived classes, this
     *  should be the called from their paint method before drawing any sources.
     *
     *  @param g The graphics context to be used for drawing.
     *
     *  @see [juce::Component](http://www.juce.com/api/classComponent.html)
     */
    void paint (Graphics& g);
    
    /** Sets the size of the dot indicating the source's position. Various other
     *  sizes are determined by this (i.e. the size of the arrow indciating a
     *  plane wave source and the text size of the source names).
     *
     *  @param diameter The dot's diameter in pixels.
     */
    void setPositionDotSize(int diameter);
    
    /** Returns the size of the dot indicating the source's position (diameter in
     *  pixels).
     *
     *  @return The size of the dot indicating the source's position (diameter in
     *      pixels).
     */
    int getPositionDotSize() const;
    
    /** Sets the wonder::Room object that is used for painting the positions of
     *  the speaker arrays.
     *
     *  @param room A (reference-counted) shared pointer to the wonder::Room
     *      object used for painting the speaker arrays.
     */
    bool setRoom(std::shared_ptr <const wonder::Room> room);
    
    /** Sets whether the names of the sources shall be shown next to them.
     *
     *  @param showNames true if the names of the sources shall be shown
     *      next to them.
     */
    void setShowNames(bool showNames);
    
    /** Returns the current showNames setting. If this is true, the names
     *  of the sources are shown.
     *
     *  @return The current showNames setting. If this is true, the names
     *      of the sources are shown.
     */
    bool showsNames() const;
    
protected:
    
    /** Paints a wonder::Source. Can be called from the paint methods of derived
     *  classes to paint sources (at their position, with their colour etc.).
     *
     *  @param g The graphics context to be used for drawing.
     *  @param source The source to be painted.
     *  @param alpha The alpha-channel value for painting the source. This can
     *      be used for drawing semi-transparent sources.
     */
    void paintSource(Graphics& g, const wonder::Source& source, uint8_t alpha = 0xFF);
    
private:
    std::shared_ptr <const wonder::Room> room_;
    bool showNames_;
    int dotSize_;
    int roomLineThickness_;
    Colour roomColour_;
    Colour bgColour_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceDisplay)
};

}

#endif  // SOURCEDISPLAY_H_INCLUDED
