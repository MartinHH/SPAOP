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

#ifndef SOURCEZOOMPORT_H_INCLUDED
#define SOURCEZOOMPORT_H_INCLUDED

#include "SourcePanel.h"
#include "ZoomPort.h"
#include "JuceHeader.h"

namespace wonderjuce {

//==============================================================================
/**
 *  A ZoomPort that contains a SourcePanel.
 */
class SourceZoomPort    : public ZoomPort
{
public:
    /** Constructor. */
    SourceZoomPort(const String& componentName = String::empty);

    /** Destructor. */
    ~SourceZoomPort();
    
    /** Returns a pointer to the contained SourcePanel.
     *
     *  @return A pointer to the SourcePanel contained in this SourceZoomPort.
     */
    SourcePanel* getSourcePanel();

private:
    SourcePanel sourcePanel_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceZoomPort)
};

}



#endif  // SOURCEZOOMPORT_H_INCLUDED
