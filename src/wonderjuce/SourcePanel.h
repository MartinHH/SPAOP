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

#define OTHER_SOURCES_ALPHA 80 /**< The alpha value used to display other
                                    sources than the main source. */

#include <memory>

#include "Room.h"
#include "SourceCollection.h"
#include "SourceDisplay.h"
#include "JuceHeader.h"

namespace wonderjuce {

//==============================================================================
/**
 *  A GUI component that displays a two-dimensional view of a source's position
 *  and allows the re-positioning by mouse actions.
 */
class SourcePanel    : public SourceDisplay
{
public:
    SourcePanel(const String& componentName = String::empty);
    
    /** Destructor. */
    virtual ~SourcePanel();

    /** @see [juce::Component](http://www.juce.com/api/classComponent.html). */
    void paint (Graphics& g);
    
    /** @see [juce::Component](http://www.juce.com/api/classComponent.html). */
    void mouseDown(const MouseEvent &event) override;
    
    /** @see [juce::Component](http://www.juce.com/api/classComponent.html). */
    void mouseDrag(const MouseEvent &event) override;
    
    /** @see [juce::Component](http://www.juce.com/api/classComponent.html). */
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
        virtual void sourcePanelDragEnded (SourcePanel* panel) {}
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
    
    float getFocusPointX();
    
    float getFocusPointY();
    
    Point<float> getFocusPoint();
    
private:
    float x, y;
    std::shared_ptr<const wonder::SourceCollection> sources_;
    bool showOthers_;
    int sourceID_;
    bool dotIsHit_;
    ListenerList <Listener> listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourcePanel)
};
    
}



#endif  // SOURCEPANEL_H_INCLUDED
