
#pragma region Includes
#include <stdio.h>
#include <windows.h>
#include "tools/serviceInstaller.h"
#include "service/serviceBase.h"
#include "service/raumserverService.h"
#pragma endregion


// Internal name of the service 
#define SERVICE_NAME             L"Raumserver" 
// Displayed name of the service 
#define SERVICE_DISPLAY_NAME     L"Raumserver" 
// Service start options. 
#define SERVICE_START_TYPE       SERVICE_DEMAND_START 
// List of service dependencies - "dep1\0dep2\0\0" 
#define SERVICE_DEPENDENCIES     L"" 
// The name of the account under which the service should run 
#define SERVICE_ACCOUNT          L"NT AUTHORITY\\LocalService" 
// The password to the service account name 
#define SERVICE_PASSWORD         NULL 


int wmain(int argc, wchar_t *argv[])
{
    if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/'))))
    {
        if (_wcsicmp(L"install", argv[1] + 1) == 0)
        {
            // Install the service when the command is  
            // "-install" or "/install". 
            InstallService(
                (LPCTSTR)SERVICE_NAME,               // Name of service 
                (LPCTSTR)SERVICE_DISPLAY_NAME,       // Name to display 
                SERVICE_START_TYPE,         // Service start type 
                (LPCTSTR)SERVICE_DEPENDENCIES,       // Dependencies 
                (LPCTSTR)SERVICE_ACCOUNT,            // Service running account 
                (LPCTSTR)SERVICE_PASSWORD            // Password of the account 
                );
        }
        else if (_wcsicmp(L"remove", argv[1] + 1) == 0)
        {
            // Uninstall the service when the command is  
            // "-remove" or "/remove". 
            UninstallService((LPCTSTR)SERVICE_NAME);
        }
    }
    else
    {
        wprintf(L"Parameters:\n");
        wprintf(L" -install  to install the service.\n");
        wprintf(L" -remove   to remove the service.\n");


        RaumserverService service(SERVICE_NAME);
        if (!RaumserverService::Run(service))
        {
            wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
        }
    }


    return 0;
}
