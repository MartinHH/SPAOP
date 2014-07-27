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

#ifndef SPAOPEDITORCOMPONENT_H_INCLUDED
#define SPAOPEDITORCOMPONENT_H_INCLUDED

#include "PluginProcessor.h"
#include "JuceHeader.h"

namespace wonderjuce {
    
    /**
     *  An abstract class for GUI subcomponents of the SpaopAudioProcessorEditor.
     *  This class can be used to group buttons, labels, textboxes etc into a component.
     *  It holds a pointer to the SpaopAudioProcessor.
     
     *  A virtual method update() must be overriden. It shall be used to update the
     *  internal components (e.g. labels) to the state of the SpaopAudioProcessor and
     *  must be called from SpaopAudioProcessorEditor::timerCallback.
     */
    class SpaopEditorComponent    : public Component
    {
    public:
        /** Constructor.
         *
         */
        SpaopEditorComponent(SpaopAudioProcessor* ownerFilter);
        
        /** Destructor. */
        virtual ~SpaopEditorComponent();
        
        /** To be called by the main SpaopAudioProcessorEditor to make this component
         *  update its values to the currents state of the SpaopAudioProcessor.
         */
        virtual void update() = 0;
        
    protected:
        
        /** Returns the SpaopAudioProcessor controlled by this editor component. */
        SpaopAudioProcessor* getProcessor() const;
        
    private:
        SpaopAudioProcessor* const processor_;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaopEditorComponent)
    };
    
}




#endif  // SPAOPEDITORCOMPONENT_H_INCLUDED
