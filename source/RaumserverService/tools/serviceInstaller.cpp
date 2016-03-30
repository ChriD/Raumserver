/****************************** Module Header ******************************\
* Module Name:  ServiceInstaller.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
* 
* The file implements functions that install and uninstall the service.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma region "Includes"
#include <stdio.h>
#include <windows.h>
#include "tools/serviceInstaller.h"
#pragma endregion


/**
* Install the current application as a service to the local service control manager database.
* 
* NOTE: If the function fails to install the service, it prints the error 
* in the standard output stream for users to diagnose the problem
*/
void InstallService(std::string  _serviceName, std::string  _displayName, std::string _description, DWORD _startType, std::string  _dependencies, std::string  _account, std::string  _password)
{    
    char *path = new char[MAX_PATH];
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;    
    SERVICE_STATUS ssSvcStatus = {};

    if (GetModuleFileName(NULL, path, MAX_PATH) == 0)
    {
        std::cout << "GetModuleFileName failed w/err " + std::to_string(GetLastError());
    }
    else
    {
        // Open the local default service control manager database
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
        if (schSCManager == NULL)        
            std::cout << "OpenSCManager failed w/err " + std::to_string(GetLastError()) + "\n";                   
        else
        {
            // Install the service into SCM by calling CreateService
            schService = CreateService(
                schSCManager,                   // SCManager database
                _serviceName.c_str(),           // Name of service
                _displayName.c_str(),           // Name to display
                SERVICE_CHANGE_CONFIG | SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP,          // Desired access
                SERVICE_WIN32_OWN_PROCESS,      // Service type
                _startType,                     // Service start type
                SERVICE_ERROR_NORMAL,           // Error control type
                path,                           // Service's binary
                NULL,                           // No load ordering group
                NULL,                           // No tag identifier
                _dependencies.c_str(),          // Dependencies
                _account.c_str(),               // Service running account
                _password.c_str()               // Password of the account
                );

            if (schService == NULL)
                std::cout << "CreateService " + _serviceName + " failed w/err " + std::to_string(GetLastError()) + "\n";   
            else
            {                
                SERVICE_DESCRIPTION description;
                description.lpDescription = const_cast<char *>(_description.c_str());            
                if(!ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &description))
                    std::cout << "Change service description for " + _serviceName + " failed w/err " + std::to_string(GetLastError()) + "\n";

                std::cout << _serviceName + " is installed.\n";               
            }
        }       
    }

    if (schSCManager)
    {
        CloseServiceHandle(schSCManager);
        schSCManager = NULL;
    }
    if (schService)
    {
        CloseServiceHandle(schService);
        schService = NULL;
    }

}


void UninstallService(std::string _serviceName)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;
    SERVICE_STATUS ssSvcStatus = {};

    // Open the local default service control manager database
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (schSCManager == NULL)
    {
        std::cout << "OpenSCManager failed w/err " + std::to_string(GetLastError()) + "\n";                
    }
    else
    {
        // Open the service with delete, stop, and query status permissions
        schService = OpenService(schSCManager, _serviceName.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
        if (schService == NULL)       
            std::cout << "OpenService failed w/err " + std::to_string(GetLastError()) + "\n";                    
        // try to stop service
        else if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
        {
            std::cout << "Stopping " + _serviceName;
            Sleep(1000);

            while (QueryServiceStatus(schService, &ssSvcStatus))
            {
                if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
                {
                    std::cout <<  ".";
                    Sleep(1000);
                }
                else break;
            }

            if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
                std::cout << "\n" + _serviceName + " is stopped.\n";
            else
                std::cout << "\n" + _serviceName + " failed to stop.\n";
        }

        // Now remove the service by calling DeleteService.
        if (!DeleteService(schService))
            std::cout << "Delete service failed w/err " + std::to_string(GetLastError()) + "\n";
        else
            std::cout << _serviceName + " is removed.\n";
    }
    
    if (schSCManager)
    {
        CloseServiceHandle(schSCManager);
        schSCManager = NULL;
    }
    if (schService)
    {
        CloseServiceHandle(schService);
        schService = NULL;
    }
}