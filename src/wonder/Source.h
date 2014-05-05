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

#ifndef SOURCE_H_INCLUDED
#define SOURCE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdint.h>
#include "Colours.h"
#include "WonderHeader.h"

#define ANGLE_MAX 360.0
#define ANGLE_MIN 0.0

#define NORMALIZE(min, max, val) ((val)-(min)) / ((max)-(min))
#define DENORMALIZE(min, max, val) ((val) * ((max)-(min))) + (min)

#define COORD_NORM(x)   NORMALIZE(COORD_MIN, COORD_MAX, (x))
#define COORD_DENORM(x) DENORMALIZE(COORD_MIN, COORD_MAX, (x))

/**
 *  A namespace for library- and system-independent WONDER-related
 *  classes.
 */
namespace wonder {

/**
 *  A class for objects representing a WONDER source with all its properties.
 *  (This is just a container storing the source's parameter values, offering
 *  a range of convenient getters and setters. To actually manipulate a source
 *  by sending OSC messages to WONDER, have a look at SourceController.)
 *
 *  Parameters that are suitable for automation can be accessed via an index
 *  system and normalized float[0.0 , 1.0] values as used by most audio plugin
 *  interfaces. Alternative setters and getters for non-normalized values are
 *  avaibale as well.
 *

 */
class Source
{
public:
    
    /** Constructor.
     *
     *  @param id The wonder-specific ID of the source. May range from 0 to N-1
     *      where N is the total number of sources supported by the current
     *      WONDER installation.
     */
    Source(int id=0);
    
    /** Destructor. */
    virtual ~Source();
    
    //==============================================================================
    // Enums:
    
    /** Parameters that can be automated by the host must be accessed using this
     *  enum.
     */
    enum AutomatedParameters
    {
        typeParam = 0,  /**< The (WONDER) source type. @see enum SourceTypes. */
        xPosParam,      /**< The source's x-axis coordinate. */
        yPosParam,      /**< The source's y-axis coordinate. */
        angleParam,     /**< The angle of a plain source. */
        dopplParam,     /**< Doppler effect (on/off). */
        
        totalNumParams  /**< Total number of (automated) parameters. This
                             does NOT represent an actual parameter. */
    };
    
    /** WONDER's different source types. */
    enum SourceTypes
    {
        plane = 0,      /**< A plane wave source. */
        point           /**< A point source. */
    };
    
    //==============================================================================
    // Methods for (automated) parameters by index:
    
    /** Returns the total numbers of parameters that can be accessed via enum
     *  AutomatedParameters.
     */
    int getNumParameters() const;
    
    /** Returns the normalized value of the parameter.
     *  
     *  @param index A parameter index as specified by enum SourceTypes.
     */
    float getParameter (int index) const;
    
    /** Sets a parameter from its normalized value.
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @param newValue the new value for that parameter, normalized within
     *      [0.0 , 1.0].
     */
    void setParameter (int index, float newValue);
    
    /** Sets a parameter from its unnormalized value.
     *  
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @param newUnnormalizedValue the new value for that parameter, not
     *      normalized.
     */
    void setParameterUnnormalized(int index, float newUnnormalizedValue);
    
    /** Returns the name of the parameter.
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @return A string describing the specified parameter.
     */
    const std::string getParameterName (int index) const;
    
    /** Returns the string representation of the parameter's current value.
     *  
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @return A string describing the current (unnormalized) value of
     *      the parameter.
     */
    const std::string getParameterText (int index) const;
    
    /** Returns the parameter's (denormalized) string representation for a
     *  given (normalized) value.
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @param value The normalized value of the specified parameter whose
     *      (unnormalized) string representaion shall be returned.
     *  @return A string describing the current (unnormalized) value of
     *      the parameter.
     */
    static std::string getParameterTextByValue (int index, float value);
    
    /** Returns the default (=initial) value of the parameter.
     *  
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @return The default (=intial) value for the specified paremeter.
     */
    static float getParameterDefaultValue (int index);
    
    /** Normalizes a parameter to the range of [0.0 ; 1.0].
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @param value The unnormalized value of the specified parameter whose
     *      normalized float representaion shall be returned.
     *  @return The normalized representation of the value that was passed.
     */
    static float normalizeParameter (int index, float unnormalized);
    
    /** Scales a parameter from the range of [0.0 ; 1.0] to the value it
     *  represents to the user (for GUI, getParameterText, ...).
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @param value The normalized value of the specified parameter whose
     *      unnormalized representaion shall be returned.
     *  @return The unnormalized representation of the value that was passed.
     */
    static float denormalizeParameter (int index, float normalized);
    
    /** Returns the denormalized value of the parameter.
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @return The current unnormalized value of that parameter.
     */
    float getDenormalizedParameter (int index) const;
    
    /** Returns false, if the parameter's value is 0.0 (otherwise true).
     *
     *  @param index A parameter index as specified by enum SourceTypes.
     *  @return false, if the parameter's value is 0.0 (otherwise true).
     */
    bool getBooleanizedParameter (int index) const;
    
    //==============================================================================
    // For convenience: getters on the same parameters by name (denormalized):
    
    /** Returns 0, if this is a "plane" source, or 1, it's a point source.
     *  @see enum SourceTypes.
     */
    int getType() const;
    
    /** Returns the x-axis coordinate of this source. */
    float getXPos() const;

    /** Returns the y-axis coordinate of this source. */
    float getYPos() const;
    
    /** Returns the angle of a plain source in degree (irrelevant for point
     *  sources).
     */
    float getAngle() const;
    
    /** Returns true, if the doppler effect is enabled. */
    bool dopplerIsEnabled() const;

    
    //==============================================================================
    // Methods for other parameters:
    
    /** Gets the WONDER source ID. */
    int getID() const;
    
    /** Sets the WONDER source ID. */
    bool setID(int id);
    
    /** Returns the source's name. */
    std::string getName() const;
    
    /** Sets the source's name. */
    void setName(const std::string& name);
    
    /** Returns the source's colour.
     *
     *  @return The source's colour.
     */
    const Colour getCoulour() const;
    
    /** Sets the source's colour (for use in GUIs). */
    void setColour(uint8_t red, uint8_t green, uint8_t blue);
    
    /** Sets the source's colour (for use in GUIs). */
    void setColour(Colour colour);
    
    /** Returns the red value of this source's colour (as in RGB).*/
    uint8_t getRed() const;

    /** Sets the red value of this source's colour (as in RGB).*/
    void setRed(uint8_t red);
    
    /** Returns the green value of this source's colour (as in RGB).*/
    uint8_t getGreen() const;
    
    /** Sets the green value of this source's colour (as in RGB).*/
    void setGreen(uint8_t green);
    
    /** Returns the blue value of this source's colour (as in RGB).*/
    uint8_t getBlue() const;
    
    /** Sets the red value of this source's colour (as in RGB).*/
    void setBlue(uint8_t blue);
    
    /** Returns true, if the source is active within WONDER. (This
     *  is just a getter for the value that was set via isActive().)
     */
    bool isActive() const;
    
    /** Sets the "source is active" status. */
    void setIsActive(bool isActive);
    
private:
    float params_[totalNumParams];      // the (normalized) parameters
    int id_;                            // the WONDER-specific source ID
    std::string name_;                  // the source's name in WONDER
    Colour colour_;                     // the colour the source is displayed in
    bool active_;                       // source has been activated
    
    /** Creates a string representing a float value. */
    static std::string floatString(float f);
};

}

#endif  // SOURCE_H_INCLUDED
