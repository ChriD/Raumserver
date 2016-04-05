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
#ifndef RAUMKERNEL_EXCEPTION_H
#define RAUMKERNEL_EXCEPTION_H

#include <exception>
#include <string>
#include <cstdint>
#include <os/os.h>

namespace Raumkernel
{
    namespace Exception
    {

        enum class ExceptionType : int8_t { EXCEPTIONTYPE_APPCRASH = 0, EXCEPTIONTYPE_RECOVERABLE = 1 };

        class RaumkernelException :public std::exception
        {
            public:
                EXPORT RaumkernelException(ExceptionType _exceptionType, const std::string _exceptionLocation, const std::string _exceptionInfo, const std::int16_t _errorCode = 999)
                {
                    exceptionTyp = _exceptionType;
                    exceptionInfo = _exceptionInfo;
                    exceptionLocation = _exceptionLocation;
                    errorCode = _errorCode;
                };           

                EXPORT ~RaumkernelException(void)
                {
                };

                EXPORT const char* what()
                {
                    return (exceptionLocation + ": " + exceptionInfo).c_str();
                };

                EXPORT ExceptionType type()
                {
                    return exceptionTyp;
                };

            private:
                std::string exceptionInfo;
                std::string exceptionLocation;   
                std::int16_t errorCode;
                ExceptionType exceptionTyp;
        };

    }
}

#endif