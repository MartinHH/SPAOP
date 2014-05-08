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

#ifndef POSMSGLOGGER_H_INCLUDED
#define POSMSGLOGGER_H_INCLUDED

#include <chrono>
#include <atomic>
#include <string>
#include <fstream>

#ifndef MAX_LOGGED_MESSAGES
#define MAX_LOGGED_MESSAGES 10000       /** The maximum number of logged messages.*/
#endif

#ifndef LOG_FILE_DIR
#define LOG_FILE_DIR "/Logs/PosLogs/"   /**< The directory where the logfile is
                                              stored. */
#endif

namespace wonder {

/**
 *  A class to log incoming our outgoing /WONDER/source/position(int, float, float)
 *  messages along with a (microseconds since epoch) timestamp.
 *  
 *  Only the first MAX_LOGGED_MESSAGES are logged (memory for logging is pre-
 *  allocated, hence there is a fixed value for that). The logfile is written on
 *  destruction of the object.
 */
class PosMsgLogger
{
public:
    PosMsgLogger(const std::string& filename, int sourceID=0);
    virtual ~PosMsgLogger();
    
    struct LogItem{
        std::chrono::time_point<std::chrono::high_resolution_clock> time;
        int sourceID;
        float x;
        float y;
    };
    
    void logPosMessage(int sourceID, float x, float y);
    
    void setSourceID(int sourceID);
    
private:
    PosMsgLogger(const PosMsgLogger& other);            // copying not allowed
    PosMsgLogger &operator= (const PosMsgLogger other); // assignment not allowed
    
    LogItem log_[MAX_LOGGED_MESSAGES];
    std::atomic_int logCount_;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    const std::string filePath_;
    int sourceID_;
};

}

#endif  // POSMSGLOGGER_H_INCLUDED
