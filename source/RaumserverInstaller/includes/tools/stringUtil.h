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
#ifndef RAUMKERNEL_STRINGUTIL_H
#define RAUMKERNEL_STRINGUTIL_H

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>

namespace Raumkernel
{
    namespace Tools
    {
        class StringUtil
        {
            public:            


                static std::string tolower(std::string _string)
                {
                    std::transform(_string.begin(), _string.end(), _string.begin(), ::tolower);
                    return _string;
                }


                static std::string toupper(std::string _string)
                {
                    std::transform(_string.begin(), _string.end(), _string.begin(), ::toupper);
                    return _string;
                }


                static std::string ltrim(std::string  _s)
                {
                    _s.erase(_s.begin(), std::find_if(_s.begin(), _s.end(), std::not1(std::ptr_fun<int, int>(::isspace))));
                    return _s;
                }


                static std::string rtrim(std::string _s)
                {
                    _s.erase(std::find_if(_s.rbegin(), _s.rend(), std::not1(std::ptr_fun<int, int>(::isspace))).base(), _s.end());
                    return _s;
                }


                static std::string trim(std::string _s)
                {
                    return ltrim(rtrim(_s));
                }


                static std::string padL(std::string _s, const size_t _num, const char _paddingChar = ' ')
                {
                    if (_num > _s.size())
                        _s.insert(0, _num - _s.size(), _paddingChar);
                    return _s;
                }


                static std::string padR(std::string _s, const size_t _num, const char _paddingChar = ' ')
                {
                    _s.resize( _num, _paddingChar);
                    return _s;
                }


                static std::string center(std::string _s, int _width)
                {
                    _s = StringUtil::ltrim(StringUtil::rtrim(_s));
                    return std::string((_width - _s.length()) / 2, ' ') + _s;
                }


                static std::uint32_t toTimeMs(const std::string &_timeString)
                {
                    // eg.: 00:04:02
                    if (_timeString.empty())
                        return 0;
                    
                    std::uint32_t timeInSeconds = 0;                    

                    try
                    {                                              
                        // Get_Time wont work with gcc compilers older than 4.9?!
                        // So we do need an alternative for that (see C code below)
                        /*
                        struct std::tm tm;
                        std::istringstream ss(_timeString);
                        ss >> std::get_time(&tm, "%H:%M:%S");                                             
                        timeInSeconds = (tm.tm_hour * 60 * 60) + (tm.tm_min * 60) + tm.tm_sec;
                        */      

                        int hh, mm, ss;
                        #ifdef _MSC_VER
                        sscanf_s(_timeString.c_str(), "%d:%d:%d", &hh, &mm, &ss);
                        #else
                        sscanf(_timeString.c_str(), "%d:%d:%d", &hh, &mm, &ss);
                        #endif
                        timeInSeconds = (hh * 60 * 60) + (mm * 60) + ss;
                    }
                    catch (...)
                    {
                        // TODO: what to do?
                    }
            
                    return timeInSeconds * 1000;
                 }


                /**
                * Used to explode a string into multiple string with the given sepaerator
                */
                static std::vector<std::string> explodeString(const std::string &inString, const std::string &separator, std::uint32_t _maxSplits = 0)
                {
                    std::vector<std::string> returnVector;
                    std::string::size_type start = 0;
                    std::string::size_type end = 0;
                    std::uint32_t splits = 0;

                    while ((end = inString.find(separator, start)) != std::string::npos) 
                    {
                        returnVector.push_back(inString.substr(start, end - start));
                        start = end + separator.size();
                        splits++;
                        if (_maxSplits && splits>=_maxSplits)
                            break;
                    }
                    returnVector.push_back(inString.substr(start));
                    return returnVector;
                }
        };
            
    }
}


#endif