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
#ifndef RAUMKERNEL_LOGGERADAPTER_H
#define RAUMKERNEL_LOGGERADAPTER_H

#include <logger/loggerBase.h>

namespace Raumkernel
{
    namespace Log
    {

        class LogAdapter
        {
        public:
            LogAdapter();
            virtual ~LogAdapter();
            virtual void log(LogData _logData);
        protected:
            std::string getLogTypeString(LogType _logType);
            std::string getFormattedCurrentDateTimeString(Raumkernel::Tools::DateTimeStamp _stamp);
        };



        class LogAdapter_File : public LogAdapter
        {
        public:
            EXPORT LogAdapter_File();
            EXPORT virtual ~LogAdapter_File();
            virtual void log(LogData _logData) override;
            virtual void setLogFilePath(const std::string &_path);

        protected:
            std::string getCurrentDateTimeFileString();

            std::string logFilePath;
            std::string logFileName;
        };



        class LogAdapter_Console : public LogAdapter
        {
        public:
            EXPORT LogAdapter_Console();
            EXPORT virtual ~LogAdapter_Console();
            virtual void log(LogData _logData) override;
        };

    }
}


#endif