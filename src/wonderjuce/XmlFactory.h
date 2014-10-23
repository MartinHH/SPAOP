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

#ifndef XMLFACTORY_H_INCLUDED
#define XMLFACTORY_H_INCLUDED

#include "Source.h"
#include "SourceController.h"
#include "SourceCollection.h"
#include "Room.h"
#include "XmlParser.h"
#include "JuceHeader.h"

namespace wonderjuce {

/**
 *  A class with static functions to create XmlElements from Objects, create
 *  Objects from XmlElements and set existing Objects to the values given in
 *  an XmlElement.
 */
class XmlFactory final
{
private:
    XmlFactory();
    ~XmlFactory();
public:
    
    /** Singleton-getter for a wonder::XmlParser implementation that wraps the 
     *  corresponding static functions of this class.
     */
    static wonder::XmlParser* getParser();
    
    /** Sets the properties of a wonder::Source object to the values given in an
     *  XML element. If the XmlElement is invalid, the Source object remains
     *  unchanged.
     *  
     *  @param sourceElement An XML element representing a WONDER source as
     *      defined in cwonder_project.dtd.
     *  @param source The Source object that shall be updated.
     */
    static void updateSourceFromXml(const XmlElement* sourceElement,
                                    wonder::Source* source);
    
    /** Sets the properties of a wonder::Source object to the values given in an
     *  XML formatted string describing a cWONDER project (as defined in
     *  cwonder_project.dtd and sent via the /WONDER/project/xmlDump message).
     *  The Source will only be changed if the Xml string is valid and contains
     *  information about a source with the same ID.
     *
     *  @param cwProjectXml An XML formatted string describing a cWONDER project
     *      (as defined in cwonder_project.dtd).
     *  @param source The Source object that shall be updated.
     */
    static void updateSourceFromCWonderProject(const std::string& cwProjectXml,
                                               wonder::Source* source);
    
    /** Sets the properties of a wonder::SourceCollection to the values given in a
     *  cWONDER project XML string
     *
     *  @param cwProjectXml An XML formatted string describing a cWONDER project
     *      (as defined in cwonder_project.dtd).
     *  @param sources The SourceCollection that shall be updated.
     */
    static void updateSourceCollectionFromCWonderProject(const std::string& cwProjectXml,
                                                         wonder::SourceCollection& sources);
    
    /** Creates an XmlElement representing a WONDER source as defined in
     *  cwonder_project.dtd.
     *
     *  @param source The source that shall be stored to XML.
     *  @return an XmlElement representing a WONDER source as defined in
     *      cwonder_project.dtd.
     */
    static XmlElement* createSourceXml(const wonder::Source& source);

    /** Creates a wonder::Room object from an XmlElement that was created by
     *  the createRoomFromXml method.
     *  
     *  @param roomElement an XmlElement as created by createRoomFromXml.
     *  @return a new wonder::Room object set to the values given in the
     *      XmlElement. The caller is responsible for deleting the Room
     *      object.
     */
    static const wonder::Room createRoomFromXml (const XmlElement* roomElement);

    /** Creates an XmlElement representing a wonder::Room object.
     *
     *  @param room The room that shall be stored to XML.
     *  @return an XmlElement containing the room info.
     */
    static XmlElement* createRoomXml (const wonder::Room& room);
    
    /** Sets the properties of a wonder::SourceController to the values given in
     *  an XmlElement that was created using the createConnectionXml method.
     *  cWONDER project XML string. The properties that are set the ip and port
     *  where outgoing OSC messages are sent.
     *
     *  @param connectionElement An XmlElement created createConnectionXml.
     *  @param controller The SourceController that shall be updated.
     */
    static void updateSourceControllerFromXml(const XmlElement* connectionElement,
                                              wonder::SourceController* controller);
    
    /** Creates an XmlElement representing a wonder::SourceController's
     *  connection state. Stored are the current ip and port where outgoing
     *  OSC messages are sent.
     *
     *  @param controller The SourceController that shall be stored to XML.
     *  @return an XmlElement containing the connection info.
     */
    static XmlElement* createConnectionXml(const wonder::SourceController* controller);
    
    /** Creates an XmlElement that holds the parameters of a SPAOP gui that need to
     *  be stored.
     *
     *  @param zoomFactor The zoom factor of the gui's SourcePanel.
     *  @param showOthers The showOthers setting of the gui's SourcePanel.
     *  @param showNames The showNames setting of the gui's SourcePanel.
     *  @return an XmlElement containing the corresponding info. The element is
     *      named "gui", its attributes are named "zoom" and "show" and "names".
     */
    static XmlElement* createGuiXml(float zoomFactor, bool showOthers, bool showNames);
    
private:
    /**
     *  An implementation of the wonder::XmlParser interface that redirects the
     *  calls to the corresponding static functions of the XmlFactory.
     */
    class JuceXmlParser :   public wonder::XmlParser
    {
    public:
        virtual ~JuceXmlParser();
        
        void updateSourceFromCWonderProject(const std::string& cwProjectXml,
                                            wonder::Source* source);
        
        void updateSourceCollectionFromCWonderProject(const std::string& cwProjectXml,
                                                      wonder::SourceCollection& sources);
    };
};

}

#endif  // XMLFACTORY_H_INCLUDED
