//
// The MIT License (MIT)
//
// Copyright (c) 2016 by ChriD
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
#ifndef RAUMKSERVERINSTALLER_H
#define RAUMKSERVERINSTALLER_H

#include <signals/signals.hpp>
#include "sciter-x-window.hpp"
#include "json/json.h"

static RECT wrc = { 100, 100, 600, 400 };

class frame : /*public sciter::window*/
    public sciter::window 
{
public:
    frame() : window(SW_MAIN | SW_ALPHA | SW_POPUP | SW_ENABLE_DEBUG, wrc) {}       

    sciter::value getNetworkAdapterInformation();

    sciter::value testCpp(json::value param1, json::value param2);

    BEGIN_FUNCTION_MAP
        FUNCTION_0("getNetworkAdapterInformation", getNetworkAdapterInformation);
        FUNCTION_2("testCpp", testCpp);        
    END_FUNCTION_MAP

};


#endif