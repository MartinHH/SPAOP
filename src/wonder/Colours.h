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

#ifndef COLOURS_H_INCLUDED
#define COLOURS_H_INCLUDED

#include <stdint.h>

namespace wonder {

/** A struct representing a (RGB) colour. */
struct Colour
{
    uint8_t r;  /**< The red value. */
    uint8_t g;  /**< The green value. */
    uint8_t b;  /**< The blue value. */
    
    /** Constructor. */
    Colour(uint8_t red, uint8_t green, uint8_t blue):
    r(red), g(green), b(blue) {}
};

/** A class that contains various Colour constants. */
class Colours
{
public:
    static Colour RED;
    static Colour GREEN;
    static Colour BLUE;
    static Colour YELLOW;
private:
    Colours();
};

}

#endif  // COLOURS_H_INCLUDED
