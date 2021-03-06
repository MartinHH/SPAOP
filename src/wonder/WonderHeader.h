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


#ifndef WONDERHEADER_H_INCLUDED
#define WONDERHEADER_H_INCLUDED

#ifndef MAX_WONDER_SOURCES
#define MAX_WONDER_SOURCES 64   /**< The maximum number of sources. */
#endif

//==============================================================================
// vvvvv source-parameter-related defines:


#ifndef COORD_MAX
#define COORD_MAX 100.0         /**< The maximum range of coordinates in meters.
                                     Coordinates can range from COORD_MIN to
                                     COORD_MAX.*/
#endif

#ifndef COORD_MIN
#define COORD_MIN -COORD_MAX    /**< The minimum range of coordinates in meters.
                                     Coordinates can range from COORD_MIN to
                                     COORD_MAX.*/
#endif

#ifndef COORD_PRECISION
#define COORD_PRECISION 0.01    /**< The coordinates resolution in degree. More precisely:
                                     the minimum difference between the last coordinate
                                     value that was sent and the current one for a
                                     new position message to be sent. */
#endif

#ifndef ANGLE_PRECISION
#define ANGLE_PRECISION 0.1     /**< The angle resolution in degree. More precisely:
                                     the minimum difference between the last angle
                                     value that was sent and the current one for a
                                     new angle message to be sent. */
#endif


//==============================================================================
// vvvvv communication-related defines:


// cWONDER_
#ifndef CWONDER_DEFAULT_IP_STR
#define CWONDER_DEFAULT_IP_STR "192.168.3.1" /**< cWONDER's IP. */
#endif

#ifndef CWONDER_DEFAULT_PORT_STR
#define CWONDER_DEFAULT_PORT_STR "58100"    /**< cWONDER's port. */
#endif

// Multicast group:
#ifndef VISUAL_MC_GROUP_STR
#define VISUAL_MC_GROUP_STR "239.0.111.111" /**< The multicast group's IP. */

#endif

#ifndef VISUAL_MC_PORT_STR
#define VISUAL_MC_PORT_STR "11111"          /**< The group's port. */
#endif

#ifndef VISUAL_MC_IFACE_IP_STR
#define VISUAL_MC_IFACE_IP_STR "127.0.0.1"  /**< The IP of the interface where
                                                 the mutlicast messages are
                                                 sent from. */
#endif

#ifndef VISUAL_MC_TTL
#define VISUAL_MC_TTL 0  /**< The time-to-live value for messages sent
                              to the multicast group. */
#endif

// the stream-multicaster for the visual stream:
#ifndef MULTICASTER_PORT_STR
#define MULTICASTER_PORT_STR "58800"    /**< The multicaster's port for receiving
                                            /WONDER/stream/<streamname>/connect
                                            messages. */
#endif

#ifndef MULTICASTER_STREAM_IN_PORT_STR
#define MULTICASTER_STREAM_IN_PORT_STR "58801"  /**< The multicaster's port for receiving
                                                    the incoming stream from cWONDER. */
#endif


#ifndef PING_TIMEOUT_INTERVAL
#define PING_TIMEOUT_INTERVAL 3000  /**< The maximum allowed interval between two
                                        ping messages (in ms). If this is exceeded,
                                        the ping is considered lost. */
#endif


namespace wonder {

/** An enum describing the differents states of an incoming connection. */
enum ConnectionStates
{
    inactive = 0,   /**< No incoming data is expected. */
    active,         /**< "Visual Stream" data is being received. */
    error           /**< Incoming data is expected but not received (= ping
                     timeout). */
};

}


#endif  // WONDERHEADER_H_INCLUDED
