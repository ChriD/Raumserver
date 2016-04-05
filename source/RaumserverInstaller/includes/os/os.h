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
#ifndef RAUMKERNEL_OS_H
#define RAUMKERNEL_OS_H


// Microsoft
#if defined(_MSC_VER)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
    #define CURRENT_FUNCTION __FUNCTION__
    #define CURRENT_LINE __LINE__    
    #define CONSOLE_COLS 79
//  GCC
#elif defined(_GCC)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
    #define CURRENT_FUNCTION __func__
    #define CURRENT_LINE __LINE__
    #define CONSOLE_COLS 0
// no idea which compiler	
#else
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
    #define CURRENT_FUNCTION __func__
    #define CURRENT_LINE __LINE__
    #define CONSOLE_COLS 0
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define CURRENT_POSITION CURRENT_FUNCTION

#endif
