/****************************** Module Header ******************************\
* Module Name:  ServiceBase.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
*
* Provides a base class for a service that will exist as part of a service
* application. CServiceBase must be derived from when creating a new service
* class.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/


#include "service/serviceBase.h"
#include <assert.h>
#include <strsafe.h>


// Initialize the singleton service instance.
CServiceBase *CServiceBase::service = NULL;


BOOL CServiceBase::run(CServiceBase &_service)
{
    service = &_service;
    
    SERVICE_TABLE_ENTRY serviceTable[] =
    {
        { const_cast<char *>(service->name.c_str()), (LPSERVICE_MAIN_FUNCTIONA)ServiceMain },
        { NULL, NULL }
    };

    // Connects the main thread of a service process to the service control 
    // manager, which causes the thread to be the service control dispatcher 
    // thread for the calling process. This call returns when the service has 
    // stopped. The process should simply terminate when the call returns.
    return StartServiceCtrlDispatcher(serviceTable);
}


std::string CServiceBase::getExecutionPath()
{
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);    
}


void WINAPI CServiceBase::ServiceMain(DWORD _argc, PWSTR *_argv)
{
    assert(service != NULL);
 
    // Register the handler function for the service
    service->statusHandle = RegisterServiceCtrlHandler(
        service->name.c_str(), ServiceCtrlHandler);
    if (service->statusHandle == NULL)
    {
        service->writeEventLogEntry("Register service control handler failed!", EVENTLOG_ERROR_TYPE);
        throw GetLastError();
    }

    // Start the service.    
    service->startService(_argc, _argv);
}


void WINAPI CServiceBase::ServiceCtrlHandler(DWORD _ctrl)
{
    switch (_ctrl)
    {
        // Available Parameters Constants for _ctrl
        //     SERVICE_CONTROL_CONTINUE
        //     SERVICE_CONTROL_INTERROGATE
        //     SERVICE_CONTROL_NETBINDADD
        //     SERVICE_CONTROL_NETBINDDISABLE
        //     SERVICE_CONTROL_NETBINDREMOVE
        //     SERVICE_CONTROL_PARAMCHANGE
        //     SERVICE_CONTROL_PAUSE
        //     SERVICE_CONTROL_SHUTDOWN
        //     SERVICE_CONTROL_STOP

        case SERVICE_CONTROL_STOP: service->stopService(); break;
        case SERVICE_CONTROL_PAUSE: service->pauseService(); break;
        case SERVICE_CONTROL_CONTINUE: service->continueService(); break;
        case SERVICE_CONTROL_SHUTDOWN: service->shutdownService(); break;
        case SERVICE_CONTROL_INTERROGATE: break;
        default: break;
    }
}


CServiceBase::CServiceBase(std::string _serviceName, bool _canStop, bool _canShutdown, bool _canPauseContinue)
{
    // Service name must be a valid string and cannot be NULL.
    name = (_serviceName.empty()) ? "" : _serviceName;
    statusHandle = NULL;
    // The service runs in its own process.
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    // The service is starting.
    status.dwCurrentState = SERVICE_START_PENDING;
    // The accepted commands of the service.
    DWORD dwControlsAccepted = 0;

    if (_canStop)
        dwControlsAccepted |= SERVICE_ACCEPT_STOP;
    if (_canShutdown)
        dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
    if (_canPauseContinue)
        dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;
    status.dwControlsAccepted = dwControlsAccepted;

    status.dwWin32ExitCode = NO_ERROR;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
}



CServiceBase::~CServiceBase(void)
{
}


void CServiceBase::startService(DWORD _argc, PWSTR *_argv)
{
    try
    {
        // Tell SCM that the service is starting.
        SetServiceStatus(SERVICE_START_PENDING);

        // Perform service-specific initialization.
        onStart(_argc, _argv);

        // Tell SCM that the service is started.
        SetServiceStatus(SERVICE_RUNNING);
    }
    catch (DWORD dwError)
    {
        // Log the error.
        writeErrorLogEntry("Service Start", dwError);

        // Set the service status to be stopped.
        SetServiceStatus(SERVICE_STOPPED, dwError);
    }
    catch (...)
    {
        // Log the error.
        writeEventLogEntry("Service failed to start.", EVENTLOG_ERROR_TYPE);

        // Set the service status to be stopped.
        SetServiceStatus(SERVICE_STOPPED);
    }
}


void CServiceBase::onStart(DWORD dwArgc, PWSTR *pszArgv)
{
}


void CServiceBase::stopService()
{
    DWORD dwOriginalState = status.dwCurrentState;
    try
    {
        // Tell SCM that the service is stopping.
        SetServiceStatus(SERVICE_STOP_PENDING);

        // Perform service-specific stop operations.
        onStop();

        // Tell SCM that the service is stopped.
        SetServiceStatus(SERVICE_STOPPED);
    }
    catch (DWORD dwError)
    {
        // Log the error.
        writeErrorLogEntry("Service Stop", dwError);

        // Set the orginal service status.
        SetServiceStatus(dwOriginalState);
    }
    catch (...)
    {
        // Log the error.
        writeEventLogEntry("Service failed to stop.", EVENTLOG_ERROR_TYPE);

        // Set the orginal service status.
        SetServiceStatus(dwOriginalState);
    }
}


void CServiceBase::onStop()
{
}


void CServiceBase::pauseService()
{
    try
    {
        // Tell SCM that the service is pausing.
        SetServiceStatus(SERVICE_PAUSE_PENDING);

        // Perform service-specific pause operations.
        onPause();

        // Tell SCM that the service is paused.
        SetServiceStatus(SERVICE_PAUSED);
    }
    catch (DWORD dwError)
    {
        // Log the error.
        writeErrorLogEntry("Service Pause", dwError);

        // Tell SCM that the service is still running.
        SetServiceStatus(SERVICE_RUNNING);
    }
    catch (...)
    {
        // Log the error.
        writeEventLogEntry("Service failed to pause.", EVENTLOG_ERROR_TYPE);

        // Tell SCM that the service is still running.
        SetServiceStatus(SERVICE_RUNNING);
    }
}


void CServiceBase::onPause()
{
}


void CServiceBase::continueService()
{
    try
    {
        // Tell SCM that the service is resuming.
        SetServiceStatus(SERVICE_CONTINUE_PENDING);

        // Perform service-specific continue operations.
        onContinue();

        // Tell SCM that the service is running.
        SetServiceStatus(SERVICE_RUNNING);
    }
    catch (DWORD dwError)
    {
        // Log the error.
        writeErrorLogEntry("Service Continue", dwError);

        // Tell SCM that the service is still paused.
        SetServiceStatus(SERVICE_PAUSED);
    }
    catch (...)
    {
        // Log the error.
        writeEventLogEntry("Service failed to resume.", EVENTLOG_ERROR_TYPE);

        // Tell SCM that the service is still paused.
        SetServiceStatus(SERVICE_PAUSED);
    }
}


void CServiceBase::onContinue()
{
}


void CServiceBase::shutdownService()
{
    try
    {
        // Perform service-specific shutdown operations.
        onShutdown();

        // Tell SCM that the service is stopped.
        SetServiceStatus(SERVICE_STOPPED);
    }
    catch (DWORD dwError)
    {
        // Log the error.
        writeErrorLogEntry("Service Shutdown", dwError);
    }
    catch (...)
    {
        // Log the error.
        writeEventLogEntry("Service failed to shut down.", EVENTLOG_ERROR_TYPE);
    }
}


void CServiceBase::onShutdown()
{
}


void CServiceBase::SetServiceStatus(DWORD dwCurrentState,
    DWORD dwWin32ExitCode,
    DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure of the service.

    status.dwCurrentState = dwCurrentState;
    status.dwWin32ExitCode = dwWin32ExitCode;
    status.dwWaitHint = dwWaitHint;

    status.dwCheckPoint = ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED)) ? 0 : dwCheckPoint++;

    // Report the status of the service to the SCM.
    ::SetServiceStatus(statusHandle, &status);
}


void CServiceBase::writeEventLogEntry(std::string pszMessage, WORD wType)
{
    HANDLE hEventSource = NULL;
    LPCSTR lpszStrings[2] = { NULL, NULL };

    hEventSource = RegisterEventSource(NULL, name.c_str());
    if (hEventSource)
    {
        lpszStrings[0] = name.c_str();
        lpszStrings[1] = pszMessage.c_str();

        // TODO: @@@
        ReportEvent(hEventSource,  // Event log handle
            wType,                 // Event type
            0,                     // Event category
            0,                     // Event identifier
            NULL,                  // No security identifier
            2,                     // Size of lpszStrings array
            0,                     // No binary data
            lpszStrings,           // Array of strings
            NULL                   // No binary data
            );

        DeregisterEventSource(hEventSource);
    }
}


void CServiceBase::writeErrorLogEntry(std::string pszFunction, DWORD dwError)
{
    std::string szMessage;  
    szMessage = pszFunction + " failed w/err " + std::to_string(dwError);
    writeEventLogEntry(szMessage, EVENTLOG_ERROR_TYPE);
}

