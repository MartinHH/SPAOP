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

#include "SourceCollection.h"

namespace wonder {

SourceCollection::SourceCollection(int maxSources):
    maxSources_(maxSources),
    sources_(maxSources)
{
    for(int i=0; i<maxSources; i++){
        sources_[i] = Source(i);
        sources_[i].setColour(initialColours[i%NUM_INITIAL_COLOURS]);
    }
}

SourceCollection::~SourceCollection()
{
}
    
const Source& SourceCollection::getSource(int sourceID) const
{
    return sources_.at(sourceID);
}
    
const int SourceCollection::getMaxSources() const
{
    return maxSources_;
}
    
bool SourceCollection::setSource(const Source &source)
{
    const int id = source.getID();
    if(idInRange(id)){
        sources_[id] = source;
        return true;
    } else {
        return false;
    }
}
    
void SourceCollection::activate(int sourceID)
{
    if(idInRange(sourceID)){
        sources_[sourceID].setIsActive(true);
    }
}

void SourceCollection::deactivate(int sourceID)
{
    if(idInRange(sourceID)){
        sources_[sourceID].setIsActive(false);
    }
}
    
void SourceCollection::setPosition(int sourceID, float x, float y)
{
    if(idInRange(sourceID)){
        sources_[sourceID].setParameterUnnormalized(Source::xPosParam, x);
        sources_[sourceID].setParameterUnnormalized(Source::yPosParam, y);
    }
}
    
void SourceCollection::setAngle(int sourceID, float angle)
{
    setParameterUnnormalized(sourceID,Source::angleParam, angle);
}
    
void SourceCollection::setType(int sourceID, int type)
{
    setParameterUnnormalized(sourceID, Source::typeParam, type);
}

void SourceCollection::setName(int sourceID, const std::string &name)
{
    if(idInRange(sourceID)){
        sources_[sourceID].setName(name);
    }
}
 
void SourceCollection::setColour(int sourceID, Colour colour)
{
    if(idInRange(sourceID)){
        sources_[sourceID].setColour(colour);
    }
}
 
void SourceCollection::setDopplerEffect(int sourceID, bool dopplerOn)
{
    setParameterUnnormalized(sourceID, Source::dopplParam, dopplerOn);
}
 
void SourceCollection::setParameterNormalized(int sourceID, int paramIndex,
                                              float normalizedValue)
{
    if(idInRange(sourceID) && paramIndex >= 0 && paramIndex < Source::totalNumParams){
        sources_[sourceID].setParameter(paramIndex, normalizedValue);
    }
}
    
void SourceCollection::setParameterUnnormalized(int sourceID, int paramIndex,
                                                float denormalizedValue)
{
    if(idInRange(sourceID) && paramIndex >= 0 && paramIndex < Source::totalNumParams){
        sources_[sourceID].setParameterUnnormalized(paramIndex, denormalizedValue);
    }
}
    
bool SourceCollection::idInRange(int sourceID) const
{
    return sourceID >=0 && sourceID < maxSources_;
}
    
static SourceCollection EMPTY_COLLECTION = SourceCollection(0);

Colour SourceCollection::initialColours[NUM_INITIAL_COLOURS] =
{
    Colours::RED,
    Colours::GREEN,
    Colours::BLUE,
    Colours::YELLOW
};
    
}