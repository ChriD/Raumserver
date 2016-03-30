
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include "tools/serviceInstaller.h"
#include "service/serviceBase.h"
#include "service/raumserverService.h"

// Internal name of the service 
#define SERVICE_NAME             "Raumserver" 
// Displayed name of the service 
#define SERVICE_DISPLAY_NAME     "Raumserver" 
// Displayed name of the service 
#define SERVICE_DESCRIPTION      "Raumfeld Multiroom Controller - Request Server" 
// Service start options. 
#define SERVICE_START_TYPE       SERVICE_AUTO_START 
// List of service dependencies - "dep1\0dep2\0\0" 
#define SERVICE_DEPENDENCIES     "" 
// The name of the account under which the service should run 
#define SERVICE_ACCOUNT          "NT AUTHORITY\\LocalService" 
// The password to the service account name 
#define SERVICE_PASSWORD         "" 


int wmain(int argc, wchar_t *argv[])
{
    if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/'))))
    {
        if (_wcsicmp(L"install", argv[1] + 1) == 0)
        {
            // Install the service when the command is "-install" or "/install". 
            InstallService(
                SERVICE_NAME,               // Name of service 
                SERVICE_DISPLAY_NAME,       // Name to display 
                SERVICE_DESCRIPTION,        // Description of Service
                SERVICE_START_TYPE,         // Service start type 
                SERVICE_DEPENDENCIES,       // Dependencies 
                SERVICE_ACCOUNT,            // Service running account 
                SERVICE_PASSWORD            // Password of the account 
                );
        }
        else if (_wcsicmp(L"remove", argv[1] + 1) == 0)
        {
            // Uninstall the service when the command is "-remove" or "/remove". 
            UninstallService((LPCTSTR)SERVICE_NAME);
        }
    }
    else
    {
        std::cout << "Parameters:\n";
        std::cout << " -install  to install the service.\n";
        std::cout << " -remove   to remove the service.\n";
        
        RaumserverService service(SERVICE_NAME);          
        if (!RaumserverService::run(service))
        {
            std::cout << "Service failed to run w/err " + std::to_string(GetLastError()) + "\n";                     
        }
        
    }


    return 0;
}
