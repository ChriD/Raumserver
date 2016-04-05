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
#ifndef RAUMKERNEL_COMMONUTIL_H
#define RAUMKERNEL_COMMONUTIL_H

#include <stdio.h>
#include <string>
#include <cstdint>
#include <random>
#include <locale>
#include <math.h>

namespace Raumkernel
{
    namespace Tools
    {
        class CommonUtil
        {
            public:

                /**
                * use it to get the UDN into a format we want to have
                */
                EXPORT static std::string formatUDN(std::string _udn)
                {
                    if (_udn.find("uuid:") != std::string::npos)
                        return _udn;
                    return "uuid:" + _udn;
                }


                /**
                * Returns a random integer value
                * not really the best.... so this number wont be unique if this is called fast
                */
                EXPORT static std::uint32_t randomUInt32()
                {
                    std::mt19937 rng;
                    std::uint32_t seed = (std::uint32_t)time(0);
                    rng.seed(seed);
                    std::uniform_int_distribution<uint32_t> uint_dist(1000000, 9999999);
                    return uint_dist(rng);
                }


                /**
                * Converts a string into a bool
                */
                EXPORT static bool toBool(std::string _s)
                {
                    try
                    {
                        return _s == "0" || _s.empty() ? false : true;
                    }
                    catch (...)
                    {
                    }
                    return false;
                }


                /**
                * Converts a string into a integer
                */
                EXPORT static std::int32_t toInt32(std::string _s)
                {
                    try
                    {
                        return std::stoi(_s);
                    }
                    catch (...)
                    {
                    }
                    return 0;
                }
               
        };

    }
}


#endif