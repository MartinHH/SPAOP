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
#define MAX_LOGGED_MESSAGES 1000
#endif

#ifndef LOG_FILE_DIR
#define LOG_FILE_DIR "~/PosLogs/"
#endif

namespace wonder {

class PosMsgLogger
{
public:
    PosMsgLogger(const std::string& filename);
    virtual ~PosMsgLogger();
    
    struct LogItem{
        std::chrono::microseconds time;
        int sourceID;
        float x;
        float y;
    };
    
    void logPosMessage(int sourceID, float x, float y);
    
private:
    PosMsgLogger(const PosMsgLogger& other);            // copying not allowed
    PosMsgLogger &operator= (const PosMsgLogger other); // assignment not allowed
    
    LogItem log_[MAX_LOGGED_MESSAGES];
    std::atomic_int logCount_;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    const std::string filePath_;
};

}

#endif  // POSMSGLOGGER_H_INCLUDED
