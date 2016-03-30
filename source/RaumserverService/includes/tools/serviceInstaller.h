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
#ifndef SERVICEINSTALLER_H
#define SERVICEINSTALLER_H

#include <string>
#include <iostream>


/**
* Install the current application as a service to the local service control manager database.
*
* NOTE: If the function fails to install the service, it prints the error
* in the standard output stream for users to diagnose the problem
*/
void InstallService(std::string  _serviceName, std::string  _displayName, std::string _description, DWORD _startType, std::string  _dependencies, std::string  _account, std::string  _password);
/**
* Stop and remove the service from the local service control manager database.
*
* NOTE: If the function fails to uninstall the service, it prints the 
* in the standard output stream for users to diagnose the problem
*/
void UninstallService(std::string _serviceName);


#endif