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

#include "Source.h"

namespace wonder {

Source::Source(int id):
    id_(id),
    name_(""),
    colour_(Colours::RED),
    active_(false)
{
    for(int i = 0; i < totalNumParams; i++)
    {
        params_[i] = getParameterDefaultValue(i); // initialize params
    }
}

Source::~Source()
{
}

//==============================================================================
// Methods for (automated) parameters by index:

int Source::getNumParameters() const
{
    return totalNumParams;
}

float Source::getParameter (int index) const
{
    if((index >= 0) && (index < totalNumParams)){
        return params_[index];
    }
    
    return 0.0;
}

void Source::setParameter (int index, float newValue)
{
    if( index >= 0 && index < totalNumParams
       && params_[index] != newValue){
        params_[index] = newValue;      // update value
    }
}

void Source::setParameterUnnormalized(int index, float newUnnormalizedValue)
{
    setParameter(index, normalizeParameter(index, newUnnormalizedValue));
}

const std::string Source::getParameterName (int index) const
{
    switch (index)
    {
        case typeParam:     return "type";
        case xPosParam:     return "xCoord";
        case yPosParam:     return "yCoord";
        case angleParam:    return "angle";
        case dopplParam:    return "doppler";
        default:            break;
    }
    
    return "";
}

const std::string Source::getParameterText (int index) const
{
    return getParameterTextByValue(index, params_[index]);
}

std::string Source::getParameterTextByValue (int index, float value)
{
    switch (index)
    {
        case typeParam:     return (value == (float) plane) ? "Plane" : "Point";
        case xPosParam:     return floatString(denormalizeParameter(index, value));
        case yPosParam:     return floatString(denormalizeParameter(index, value));
        case angleParam:    return floatString(denormalizeParameter(index, value));
        case dopplParam:    return value ? "active"     : "inactive";
        default:            break;
    }
    
    return "";
}
    
float Source::getParameterByText(int index, const std::string& parameterText) const
{
    switch (index)
    {
        case typeParam:
            if (parameterText.compare("Plane") == 0) {
                return (float) plane;
            } else if (parameterText.compare("Point") == 0){
                return (float) point;
            }
            break;
        case xPosParam:
        case yPosParam:
        case angleParam:
            try {
                return normalizeParameter(index, std::stof(parameterText));
            } catch (const std::invalid_argument& ia) {
                // nothing to do here...
            }
            break;
        case dopplParam:
            if (parameterText.compare("active") == 0) {
                return (float) true;
            } else if (parameterText.compare("inactive") == 0){
                return (float) false;
            }
            break;
        default:
            break;
    }
    
    return getParameter(index);
}
    
float Source::getParameterDefaultValue(int index)
{
    switch (index)
    {
        case typeParam:     return (float) point;
        case xPosParam:     return normalizeParameter(xPosParam, 0.0);
        case yPosParam:     return normalizeParameter(yPosParam, 0.0);
        case angleParam:    return normalizeParameter(angleParam, 0.0);
        case dopplParam:    return (float) false;
        default:            break;
    }
    
    return 0.0;
}

float Source::normalizeParameter(int index, float unnormalized)
{
    float rv = 0.0;
    
    switch (index)
    {
        case typeParam:
            rv = (unnormalized == (float)plane) ? (float)plane : (float)point;
            break;
        case xPosParam:
            rv = NORMALIZE(COORD_MIN, COORD_MAX, unnormalized);
            break;
        case yPosParam:
            rv = NORMALIZE(COORD_MIN, COORD_MAX, unnormalized);
            break;
        case angleParam:
            rv = NORMALIZE(ANGLE_MIN, ANGLE_MAX, unnormalized);
            break;
        case dopplParam:
            rv = unnormalized ? (float) true : (float) false;
            break;
        default:
            break;
    }
    
    return SATURATE(rv); // SATURATE ensures that the value will not exceed [0.0 ; 1.0]
}

float Source::denormalizeParameter(int index, float normalized)
{
    switch (index)
    {
        case typeParam:     return (normalized == (float)plane) ? (float)plane : (float)point;
        case xPosParam:     return DENORMALIZE(COORD_MIN, COORD_MAX, normalized);
        case yPosParam:     return DENORMALIZE(COORD_MIN, COORD_MAX, normalized);
        case angleParam:    return DENORMALIZE(ANGLE_MIN, ANGLE_MAX, normalized);
        case dopplParam:    return normalized ? (float) true : (float) false;
        default:            break;
    }
    
    return 0.0;
}

float Source::getDenormalizedParameter(int index) const
{
    return denormalizeParameter(index, getParameter(index));
}

bool Source::getBooleanizedParameter(int index) const
{
    return (bool) params_[index];
}

//==============================================================================
// For convenience: getters on the same parameters by name (denormalized):

int Source::getType() const
{
    return (int)getDenormalizedParameter(typeParam);
}

float Source::getXPos() const
{
    return getDenormalizedParameter(xPosParam);
}

float Source::getYPos() const
{
    return getDenormalizedParameter(yPosParam);
}

float Source::getAngle() const
{
    return getDenormalizedParameter(angleParam);
}

bool Source::dopplerIsEnabled() const
{
    return getBooleanizedParameter(dopplParam);
}

//==============================================================================
// Methods for other parameters: 

int Source::getID() const
{
    return id_;
}

bool Source::setID(int id)
{
    if(id >= 0 /* && id <= WONDER_ID_MAX TODO...*/){
        id_ = id;
        return true;
    } else {
        return false;
    }
}
    
const Colour Source::getCoulour() const
{
    return colour_;
}
    
std::string Source::getName() const
{
    return name_;
}

void Source::setName(const std::string& name)
{
    name_ = name;
}
    
void Source::setColour(uint8_t red, uint8_t green, uint8_t blue)
{
    colour_.r = red;
    colour_.g = green;
    colour_.b = blue;
}
    
void Source::setColour(Colour colour)
{
    colour_ = colour;
}

uint8_t Source::getRed() const
{
    return colour_.r;
}
    
void Source::setRed(uint8_t red)
{
    colour_.r = red;
}

uint8_t Source::getGreen() const
{
    return colour_.g;
}

void Source::setGreen(uint8_t green)
{
    colour_.g = green;
}
    
uint8_t Source::getBlue() const
{
    return colour_.b;
}
    
void Source::setBlue(uint8_t blue)
{
    colour_.b = blue;
}
    
bool Source::isActive() const
{
    return active_;
}
    
void Source::setIsActive(bool isActive)
{
    active_ = isActive;
}

std::string Source::floatString(float f)
{
    std::stringstream str;
    str <<  std::fixed << std::setprecision(2) << f;
    return str.str();
}

}
