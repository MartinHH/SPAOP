/*
  ==============================================================================

    SourceDisplay.h
    Created: 29 May 2014 7:22:50pm
    Author:  Smart

  ==============================================================================
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
 *  The methods of ComponentWithFocusPoint and desired reactions to mouse-actions
 *  must be implemented in a derived class.
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
    
    /** Destrcutor. */
    virtual ~SourceDisplay();
    
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
    
    bool setRoom(const wonder::Room* room);
    
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
    void paintSource(Graphics& g, const wonder::Source& source, uint8_t alpha);
    
private:
    const wonder::Room* room_;
    bool showNames_;
    int dotSize_;
    int roomLineThickness_;
    Colour roomColour_;
    Colour bgColour_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceDisplay)
};

}

#endif  // SOURCEDISPLAY_H_INCLUDED
