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

#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <iostream>
#include <vector>
#include "WonderHeader.h"

namespace wonder {

/**
 *  A class containing the wonder-specific information about a room (name and
 *  vertices).
 *  (Within WONDER, the "room" is actually a description of the positioning
 *  of the WFS speaker array.)
 */
class Room
{
public:
    
    /** A struct describing a vertex with its three coordinates. */
    struct Vertex
    {
        float x;    /**< The x-coordinate. */
        float y;    /**< The y-coordinate. */
        float z;    /**< The z-coordinate. */
        
        /** Constructor. */
        Vertex(): x(0), y(0), z(0){}
        
        /** Constructor. */
        Vertex(float xCoord, float yCoord, float zCoord):
            x(xCoord), y(yCoord), z(zCoord){}
    };
    
    /** Constructor.
     *  
     *  @param name The name of the room.
     *  @param nOfVertices The number of vertices.
     */
    Room(const std::string &name="", int nOfVertices = 0);
    
    /** Destructor.*/
    virtual ~Room();
    
    /** The name of the room as provided by cWONDER. (cWONDER reads it
     *  from a config file.)
     */
    const std::string& getName() const;
    
    /** Returns the number of vertices of the room. */
    int getNumberOfVertices() const;
    
    /** Returns a Vertex of the Room.
     *
     *  @param vertexNo An index within [0, numberOfVerices-1].
     *
     *  @return The vertex indicated by vertexNo. If vertexNo is < 0,
     *      vertex number 0 will be returned. If vertexNo is >=
     *      numberOfVerices, vertex number numberOfVertices-1 will be
     *      returned.
     */
    const Vertex& getVertex(int vertexNo) const;
    
    /** Updates a vertex of the room.
     *
     *  @param vertexNo An index within [0, numberOfVerices-1]. If vertexNo
     *      is not within that interval, nothing will be changed.
     *  @param vertex A Vertex with the new coordinates of vertex number
     *      vertexNo.
     */
    void setVertex(int vertexNo, const Vertex vertex);
    
private:
    std::string name_;
    std::vector<Vertex> vertices_;          // vertices
};

}

#endif  // ROOM_H_INCLUDED
