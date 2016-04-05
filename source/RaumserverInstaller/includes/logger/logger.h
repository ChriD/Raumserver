//
// The MIT License (MIT)
//
// Copyright (c) 2015 by ChriD
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef RAUMKERNEL_LOGGER_H
#define RAUMKERNEL_LOGGER_H

#include <logger/loggerBase.h>
#include <logger/loggerAdapter.h>


namespace Raumkernel
{
    namespace Log
    {

        class Log
        {
        public:
            EXPORT Log();
            EXPORT virtual ~Log();
            EXPORT void registerAdapter(std::shared_ptr<LogAdapter> _logAdapter);
            EXPORT void addLog(LogType _logType, const std::string &_log, const std::string &_location);
            EXPORT void setLogLevel(LogType _logTypeLevel);
            EXPORT void debug(const std::string &_log, const std::string &_location);
            EXPORT void warning(const std::string &_log, const std::string &_location);
            EXPORT void info(const std::string &_log, const std::string &_location);
            EXPORT void error(const std::string &_log, const std::string &_location);
            EXPORT void critical(const std::string &_log, const std::string &_location);
            EXPORT static LogType logTypeStringToLogType(std::string _logTypeString);

            sigs::signal<void(LogData)> sigLog;

        protected:
            std::string getLogTypeString(LogType _logType);

            // use this one to define the lowest log level that should be logged 
            LogType			logTypeLevel;

            // a mutex that will secure our log so that it will be thread save
            std::mutex		mutexLog;

            // a list of shared pointers to different log adapters which are registered to the log 
            std::vector<std::shared_ptr<LogAdapter>> logAdapterList;
        };

    }
}

#endif