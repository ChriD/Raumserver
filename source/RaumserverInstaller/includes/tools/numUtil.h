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
#ifndef RAUMKERNEL_NUMUTIL_H
#define RAUMKERNEL_NUMUTIL_H

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
        class NumUtil
        {
            public:            

                static std::uint32_t toUInt32(std::string _s)
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