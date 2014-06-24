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

#ifndef SOURCECOLLECTION_H_INCLUDED
#define SOURCECOLLECTION_H_INCLUDED


#include <mutex>
#include <memory>
#include <vector>
#include "Source.h"

#define NUM_INITIAL_COLOURS 4

namespace wonder {

/**
 *  A container class holding multiple sources (IDs 0 to maxSources-1),
 *  accessible via their sourceIDs.
 */
class SourceCollection
{
public:
    
    /** Constructor.
     *
     *  @param maxSources The number of Source objects that this
     *      SourceCollection will contain.
     */
    SourceCollection(int maxSources = MAX_WONDER_SOURCES);
    
    /** Destructor. */
    virtual ~SourceCollection();
    
    /** Returns a reference to one of the internal Source objects
     *  of this SourceCollection.
     *
     *  @param sourceID the ID of the Source that shall be returned.
     *
     *  @return A reference to the Source object with the specfied
     *      sourceID.
     *
     *  @throws std::out_of_range If the sourceID is out of range, the vector
     *      used internally will throw std::out_of_range.
     */
    const Source& getSource(int sourceID) const;
    
    /** Returns the number of Source objects that this SourceCollection
     *  contains.
     *
     *  @return The number of Source objects that this SourceCollection
     *  contains.
     */
    const int getMaxSources() const;
    
    /** Copies the parameters of a given Source object to the internal
     *  Source object with the same ID (if the ID is not out of range).
     *
     *  @return true on success - false if the Source object's parameters
     *      could not be copied because the source's ID was out of range.
     *
     *  @warning Take extra care that you really want to copy all of
     *      a Source's paramters (e.g. including isActive()).
     */
    bool setSource(const Source &source);
    
    /** Sets a source's state to active.
     *
     *  @param sourceID The ID of the source that shall be updated.
     */
    void activate(int sourceID);
    
    /** Sets a source's state to not active.
     *
     *  @param sourceID The ID of the source that shall be updated.
     */
    void deactivate(int sourceID);
    
    /** Sets a source's position parameters.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param x The unnormalized x-Coordinate to be set.
     *  @param y The unnormalized y-Coordinate to be set.
     */
    void setPosition(int sourceID, float x, float y);
    
    /** Sets a source's angle parameter.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param angle The unnormalized angle to be set (in degree).
     */
    void setAngle(int sourceID, float angle);

    /** Sets a source's type parameter.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param type The source's type to be set.
     *
     *  @see enum SourceTypes
     */ 
    void setType(int sourceID, int type);
    
    /** Sets a source's name.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param name The name to be set.
     */
    void setName(int sourceID, const std::string &name);

    /** Sets a source's colour.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param colour The colour to be set.
     */
    void setColour(int sourceID, Colour colour);
    
    /** Sets a source's doppler effect setting.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param dopplerOn true if the doppler effect is activated.
     */
    void setDopplerEffect(int sourceID, bool dopplerOn);
    
    /** Sets a parameter from its normalized value.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param paramIndex A parameter index as specified by enum SourceTypes.
     *  @param normalizedValue the new value for that parameter, normalized
     *      within [0.0 , 1.0].
     */
    void setParameterNormalized(int sourceID, int paramIndex,
                                float normalizedValue);
    
    /** Sets a source's parameter from its unnormalized value.
     *
     *  @param sourceID The ID of the source that shall be updated.
     *  @param paramIndex A parameter index as specified by enum SourceTypes.
     *  @param unnormalizedValue the new value for that parameter, not
     *      normalized.
     */
    void setParameterUnnormalized(int sourceID, int paramIndex,
                                  float unnormalizedValue);
    
private:
    bool idInRange(int sourceID) const;
    
    const int maxSources_;
    std::vector<Source> sources_;
    
    static Colour initialColours[NUM_INITIAL_COLOURS];
};

}

#endif  // SOURCECOLLECTION_H_INCLUDED
