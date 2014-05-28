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
    ZoomPort(componentName),
    sourcePanel_()
{
    setWrappedComponent(&sourcePanel_);
}

SourceZoomPort::~SourceZoomPort()
{
}

SourcePanel* SourceZoomPort::getSourcePanel()
{
    return &sourcePanel_;
}

}
