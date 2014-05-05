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

#ifndef XMLPARSER_H_INCLUDED
#define XMLPARSER_H_INCLUDED

namespace wonder {

/**
 *  An interface defining a class that sets the parameters of one or several
 *  sources from an XML formatted string describing a cWONDER project (as defined
 *  in cwonder_project.dtd).
 */
class XmlParser
{
public:
    
    /** Destructor. */
    virtual ~XmlParser(){}
    
    /** Sets the properties of a Source object to the values given in a
     *  cWONDER project XML string (if the string contains information
     *  for a source with the same ID).
     *
     *  @param cwProjectXml An XML formatted string describing a cWONDER project
     *      (as defined in cwonder_project.dtd).
     *  @param source The Source object that shall be updated.
     */
    virtual void updateSourceFromCWonderProject(const std::string& cwProjectXml,
                                                Source* source) = 0;
    
    /** Sets the properties of a SourceCollection to the values given in a
     *  cWONDER project XML string.
     *
     *  @param cwProjectXml An XML formatted string describing a cWONDER project
     *      (as defined in cwonder_project.dtd).
     *  @param sources The SourceCollection that shall be updated.
     */
    virtual void updateSourceCollectionFromCWonderProject(const std::string& cwProjectXml,
                                                          SourceCollection& sources) = 0;
};

}

#endif  // XMLPARSER_H_INCLUDED
