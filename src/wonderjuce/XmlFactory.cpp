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

#include "XmlFactory.h"

namespace wonderjuce {
    
wonder::XmlParser* XmlFactory::getParser()
{
    static XmlFactory::JuceXmlParser p; // thread-safe in C++11
    return &p;
}

void XmlFactory::updateSourceFromXml(const XmlElement* sourceElement, wonder::Source *source)
{
    if (sourceElement == nullptr || source == nullptr
        || !sourceElement->hasTagName ("source")){
        return;
    }
    
    source->setID(sourceElement->getIntAttribute("id", source->getID()));
    source->setName(sourceElement->getStringAttribute("name",
                                            source->getName()).toStdString());
        
    source->setParameterUnnormalized(wonder::Source::typeParam,
                            sourceElement->getIntAttribute("type", source->getType()));
    source->setParameterUnnormalized(wonder::Source::xPosParam,
                            sourceElement->getDoubleAttribute("posx", source->getXPos()));
    source->setParameterUnnormalized(wonder::Source::yPosParam,
                            sourceElement->getDoubleAttribute("posy", source->getYPos()));
    source->setParameterUnnormalized(wonder::Source::angleParam,
                            sourceElement->getDoubleAttribute("angle", source->getAngle()));
        
    source->setRed  (sourceElement->getIntAttribute("colorR", source->getRed()));
    source->setGreen(sourceElement->getIntAttribute("colorG", source->getGreen()));
    source->setBlue (sourceElement->getIntAttribute("colorB", source->getBlue()));
    
    source->setParameterUnnormalized(wonder::Source::dopplParam,
                            sourceElement->getIntAttribute("doppler", (int)source->dopplerIsEnabled()));

}

void XmlFactory::updateSourceFromCWonderProject(const std::string& cwProjectXml, wonder::Source* source)
{
    ScopedPointer<XmlElement> proj = XmlDocument::parse(cwProjectXml);
    
    if (source == nullptr || proj == nullptr || !proj->hasTagName("cwonder_project")) {
        return;
    }
    
    XmlElement* scenario = proj->getChildByName("scenario");
    if (scenario == nullptr) {
        return;
    }
    
    XmlElement* sourceElem = scenario->getChildByName("source");
    while(sourceElem != nullptr && sourceElem->getIntAttribute("id") != source->getID()){
        sourceElem = sourceElem->getNextElementWithTagName("source");
    }
    
    if(sourceElem != nullptr){
        XmlFactory::updateSourceFromXml(sourceElem, source);
    }
    
    // if the source is contained in the cWONDER project dump, it is active:
    source->setIsActive(true);

}
    
void XmlFactory::updateSourceCollectionFromCWonderProject(const std::string& cwProjectXml,
                                                          wonder::SourceCollection& sources)
{
    ScopedPointer<XmlElement> proj = XmlDocument::parse(cwProjectXml);
    
    if (proj == nullptr || !proj->hasTagName("cwonder_project")) {
        return;
    }
    
    XmlElement* scenario = proj->getChildByName("scenario");
    if (scenario == nullptr) {
        return;
    }
    
    wonder::Source tempSource;
    
    XmlElement* sourceElem = scenario->getChildByName("source");
    while(sourceElem != nullptr){

        updateSourceFromXml(sourceElem, &tempSource);
        
        // if the source is contained in the cWONDER project dump, it is active:
        tempSource.setIsActive(true);
        
        // update the SourceCollection:
        sources.setSource(tempSource);
        
        sourceElem = sourceElem->getNextElementWithTagName("source");
    }
}


XmlElement* XmlFactory::createSourceXml(const wonder::Source& source)
{
    // Create an outer XML element..
    XmlElement* xml = new XmlElement("source");
    
    xml->setAttribute("id",          source.getID());
    xml->setAttribute("name",        source.getName());
    xml->setAttribute("type",        source.getType());
    xml->setAttribute("posx",        source.getXPos());
    xml->setAttribute("posy",        source.getYPos());
    xml->setAttribute("angle",       source.getAngle());
    xml->setAttribute("colorR",      source.getRed());
    xml->setAttribute("colorG",      source.getGreen());
    xml->setAttribute("colorB",      source.getBlue());
    xml->setAttribute("doppler",     (int) source.dopplerIsEnabled());
    
    return xml;
}

XmlElement* XmlFactory::createRoomXml(const wonder::Room& room)
{
    XmlElement* roomXml = new XmlElement("room");
    
    roomXml->setAttribute("name", room.getName());
    
    const int nOfVerts = room.getNumberOfVertices();
    roomXml->setAttribute("numOfVertices", room.getNumberOfVertices());
    
    if (nOfVerts > 0) {
        for (int i = 0; i < nOfVerts; i++) {
            const wonder::Room::Vertex vertex = room.getVertex(i);
            XmlElement* vertXml = new XmlElement("vertex");
            vertXml->setAttribute("x", vertex.x);
            vertXml->setAttribute("y", vertex.y);
            vertXml->setAttribute("z", vertex.z);
            
            roomXml->addChildElement(vertXml);
        }
    }
    
    return roomXml;
}

const wonder::Room XmlFactory::createRoomFromXml(const XmlElement *roomElement)
{
    if (roomElement == nullptr || !roomElement->hasTagName("room")) {
        return wonder::Room();
    }
    
    std::string name = roomElement->getStringAttribute("name").toStdString();
    int nOfVerts = roomElement->getIntAttribute("numOfVertices", 0);
    
    wonder::Room room(name, nOfVerts);
        
    XmlElement* vertXml = roomElement->getChildByName("vertex");
    int i = 0;
    
    while(vertXml != nullptr && i < room.getNumberOfVertices()){
        wonder::Room::Vertex vertex(vertXml->getDoubleAttribute("x"),
                                    vertXml->getDoubleAttribute("y"),
                                    vertXml->getDoubleAttribute("z"));
        room.setVertex(i, vertex);
        vertXml = vertXml->getNextElementWithTagName("vertex");
        i++;
    }
    return room;
}

void XmlFactory::updateSourceControllerFromXml(const XmlElement *connectionElement,
                                               wonder::SourceController* controller)
{
    if (connectionElement == nullptr || controller == nullptr
        || !connectionElement->hasTagName("connection")) {
        return;
    }
    
    const String ip = connectionElement->getStringAttribute("cwonderip", controller->getCWonderHost());
    const String port = connectionElement->getStringAttribute("cwonderport", controller->getCWonderPort());
    controller->setCWonderAddress(ip.toStdString(), port.toStdString());
}

XmlElement* XmlFactory::createConnectionXml(const wonder::SourceController *controller)
{
    XmlElement* controllerXml = new XmlElement("connection");
    controllerXml->setAttribute("cwonderip", controller->getCWonderHost());
    controllerXml->setAttribute("cwonderport", controller->getCWonderPort());
    return controllerXml;
}
    
XmlElement* XmlFactory::createGuiXml(float zoomFactor, bool showOthers, bool showNames)
{
    XmlElement* guiXml = new XmlElement("gui");
    guiXml->setAttribute("zoom", zoomFactor);
    guiXml->setAttribute("show", showOthers);
    guiXml->setAttribute("names", showNames);
    return guiXml;
}
    
    
    
XmlFactory::JuceXmlParser::~JuceXmlParser()
{
}

void XmlFactory::JuceXmlParser::updateSourceFromCWonderProject(const std::string& cwProjectXml,
                                                               wonder::Source *source)
{
    XmlFactory::updateSourceFromCWonderProject(cwProjectXml, source);
}
    
void XmlFactory::JuceXmlParser::updateSourceCollectionFromCWonderProject(const std::string& cwProjectXml,
                                                                        wonder::SourceCollection& sources)

{
    XmlFactory::updateSourceCollectionFromCWonderProject(cwProjectXml, sources);
    
}

}