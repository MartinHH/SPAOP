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

#ifndef SOURCEPANEL_H_INCLUDED
#define SOURCEPANEL_H_INCLUDED

#define ROOM_LINE_THICKNESS 2
#define OTHER_SOURCES_ALPHA 80

#include <memory>

#include "Room.h"
#include "SourceCollection.h"
#include "JuceHeader.h"

namespace wonderjuce {

//==============================================================================
/**
 *  A GUI component that displays a two-dimensional view of a source's position
 *  and allows the re-positioning by mouse actions.
 */
class SourcePanel    : public Component
{
public:
    SourcePanel(const String& componentName = String::empty);
    
    virtual ~SourcePanel();

    void paint (Graphics& g);
    
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

    /** Returns the x coordinate of the source controlled by this SourcePanel,
     *  normalized within [0.0 ; 1.0].
     *
     *  @return The x coordinate of the source controlled by this SourcePanel,
     *      normalized within [0.0 ; 1.0].
     */
    float getXPos();
    
    /** Returns the y coordinate of the source controlled by this SourcePanel,
     *  normalized within [0.0 ; 1.0].
     *
     *  @return The y coordinate of the source controlled by this SourcePanel,
     *      normalized within [0.0 ; 1.0].
     */
    float getYPos();
    
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


    /** A class for receiving callbacks from a SourcePanel.
     *  To be told when a SourcePanel's value changes, you can register an
     *  SourcePanel::Listener object using SourcePanel::addListener().
     *
     *  @see SourcePanel::addListener, SourcePanel::removeListener
     */
    class Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener() {}
        
        /** Called when the position-dot is about to be dragged.
         *  This is called when a drag begins, then it's followed by multiple calls
         *  to sliderValueChanged(), and then sliderDragEnded() is called after the
         *  user lets go.
         *
         *  @param panel A pointer to the SourcePanel that initiated the call to this
         *      method.
         */
        virtual void sourcePanelDragStarted (SourcePanel* panel) {}
        
        /** Called when the SourcePanel's value is changed. This would be caused by dragging
         *  the position-dot.
         *  You can get the new values using SourcePanel::getX() and SourcePanel::getY().
         *
         *  @param panel A pointer to the SourcePanel that initiated the call to this
         *      method.
         */
        virtual void sourcePanelValuesChanged (SourcePanel* panel) = 0;
        
        /** Called after a drag operation has finished.
         *
         *  @param panel A pointer to the SourcePanel that initiated the call to this
         *      method.
         */
        virtual void sourcePanelDragEnded (SourcePanel*) {}
    };
    
    /** Adds a listener to be called when this panel's values change. */
    void addListener (Listener* listener);
    
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
    
    /** Sets the SourceCollection object that holds the information about the
     *   sources to be displayed. This may only be set once.
     *
     *  @param sources A (reference-counted) std::shared_ptr pointing to the
     *      SourceCollection object.
     *  @return true on success, false otherwise.
     */
    bool setSources(std::shared_ptr<const wonder::SourceCollection> sources);
    
    /** Sets the ID of the source to be shown as the main source that can be
     *  controlled using this SourcePanel.
     *
     *  @param sourceID The ID of the source to be shown as the main source.
     *  @return true on success, false on failure (= if the sourceID was out
     *      of range).
     */
    bool setSource(int sourceID);
    
    bool setRoom(const wonder::Room* room);
    
    /** Sets whether the other sources (not controlled by this panel) shall be
     *  shown.
     *
     *  @param showOthers true if the other sources (not controlled by this
     *      panel) shall be shown.
     */
    void setShowOtherSources(bool showOthers);
    
    /** Returns the current showOtherSources setting. If this is true, the
     *  other sources (not controlled by this panel) are shown.
     *
     *  @return The current showOtherSources setting. If this is true, the
     *      other sources (not controlled by this panel) are shown.
     */
    bool showsOtherSources() const;
    
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
    float x, y;
    std::shared_ptr<const wonder::SourceCollection> sources_;
    const wonder::Room* room_;
    bool showOthers_;
    bool showNames_;
    int sourceID_;
    int dotSize_;
    bool dotIsHit_;
    ListenerList <Listener> listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourcePanel)
};
    
}



#endif  // SOURCEPANEL_H_INCLUDED
