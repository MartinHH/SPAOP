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

#include "Room.h"

namespace wonder {

Room::Room(const std::string &name, int nOfVertices):
name_(name), vertices_(nOfVertices)
{
}

Room::~Room()
{
}

const std::string& Room::getName() const
{
    return name_;
}

int Room::getNumberOfVertices() const
{
    return vertices_.size();
}
    
const Room::Vertex& Room::getVertex(int vertexNo) const
{
    if(vertexNo < 0){
        return vertices_.front();
    } else if(vertexNo >= vertices_.size()){
        return vertices_.back();
    } else {
        return vertices_[vertexNo];
    }
}
    
void Room::setVertex(int vertexNo, const Vertex vertex)
{
    if(vertexNo >= 0 && vertexNo < vertices_.size()){
        vertices_[vertexNo] = vertex;
    }
}

}
