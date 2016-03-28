/****************************** Module Header ******************************\
* Module Name:  SampleService.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
* 
* Provides a sample service class that derives from the service base class - 
* CServiceBase. The sample service logs the service start and stop 
* information to the Application event log, and shows how to run the main 
* function of the service in a thread pool worker thread.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/


#include "service/raumserverService.h"


RaumserverService::RaumserverService(std::string pszServiceName,
                               BOOL fCanStop, 
                               BOOL fCanShutdown, 
                               BOOL fCanPauseContinue)
: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
    m_fStopping = FALSE;

    // Create a manual-reset event that is not signaled at first to indicate 
    // the stopped signal of the service.
    m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStoppedEvent == NULL)
    {
        WriteEventLogEntry("Raumserver Service creation error!", EVENTLOG_ERROR_TYPE);
        throw GetLastError();
    }
}


RaumserverService::~RaumserverService(void)
{
    if (m_hStoppedEvent)
    {
        CloseHandle(m_hStoppedEvent);
        m_hStoppedEvent = NULL;
    }
}


//
//   FUNCTION: CSampleService::OnStart(DWORD, LPWSTR *)
//
//   PURPOSE: The function is executed when a Start command is sent to the 
//   service by the SCM or when the operating system starts (for a service 
//   that starts automatically). It specifies actions to take when the 
//   service starts. In this code sample, OnStart logs a service-start 
//   message to the Application log, and queues the main service function for 
//   execution in a thread pool worker thread.
//
//   PARAMETERS:
//   * dwArgc   - number of command line arguments
//   * lpszArgv - array of command line arguments
//
//   NOTE: A service application is designed to be long running. Therefore, 
//   it usually polls or monitors something in the system. The monitoring is 
//   set up in the OnStart method. However, OnStart does not actually do the 
//   monitoring. The OnStart method must return to the operating system after 
//   the service's operation has begun. It must not loop forever or block. To 
//   set up a simple monitoring mechanism, one general solution is to create 
//   a timer in OnStart. The timer would then raise events in your code 
//   periodically, at which time your service could do its monitoring. The 
//   other solution is to spawn a new thread to perform the main service 
//   functions, which is demonstrated in this code sample.
//
void RaumserverService::onStart(DWORD dwArgc, LPWSTR *lpszArgv)
{
    // Log a service start message to the Application log.
    WriteEventLogEntry("Raumserver service in OnStart", EVENTLOG_INFORMATION_TYPE);
    // Queue the main service function for execution in a worker thread.
    CThreadPool::QueueUserWorkItem(&RaumserverService::serviceWorkerThread, this);
}


//
//   FUNCTION: CSampleService::ServiceWorkerThread(void)
//
//   PURPOSE: The method performs the main function of the service. It runs 
//   on a thread pool worker thread.
//
void RaumserverService::serviceWorkerThread(void)
{    
    WriteEventLogEntry("Raumserver worker thread started", EVENTLOG_INFORMATION_TYPE);
   
    std::string executionPath = getExecutionPath();
    WriteEventLogEntry("Raumserver working directory is: " + executionPath, EVENTLOG_INFORMATION_TYPE);
    auto settingsFile = executionPath + "\\raumserver.xml";
    auto logFileDirectory = executionPath + "\\logs\\";

    WriteEventLogEntry("Raumserver settings file is: " + settingsFile, EVENTLOG_INFORMATION_TYPE);
    WriteEventLogEntry("Raumserver log file directory is: " + logFileDirectory, EVENTLOG_INFORMATION_TYPE);

    // create raumserver object adn do init init
    raumserverObject = std::unique_ptr<Raumserver::Raumserver>(new Raumserver::Raumserver());        
    raumserverObject->setSettingsFile(settingsFile);
    raumserverObject->setLogFilePath(logFileDirectory);
    
    connections.connect(raumserverObject->getLogObject()->sigLog, this, &RaumserverService::onLog);

    raumserverObject->init();
   

    // Periodically check if the service is stopping.
    while (!m_fStopping)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));      
    }

    // Signal the stopped event.
    SetEvent(m_hStoppedEvent);
}


//
//   FUNCTION: CSampleService::OnStop(void)
//
//   PURPOSE: The function is executed when a Stop command is sent to the 
//   service by SCM. It specifies actions to take when a service stops 
//   running. In this code sample, OnStop logs a service-stop message to the 
//   Application log, and waits for the finish of the main service function.
//
//   COMMENTS:
//   Be sure to periodically call ReportServiceStatus() with 
//   SERVICE_STOP_PENDING if the procedure is going to take long time. 
//
void RaumserverService::onStop()
{
    // Log a service stop message to the Application log.
    WriteEventLogEntry("Raumserver service in OnStop", EVENTLOG_INFORMATION_TYPE);

    // Indicate that the service is stopping and wait for the finish of the 
    // main service function (ServiceWorkerThread).
    m_fStopping = TRUE;
    if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
    {
        throw GetLastError();
    }
}


void RaumserverService::onLog(Raumkernel::Log::LogData _logData)
{
    WORD logTypeWord = EVENTLOG_INFORMATION_TYPE;

    if (_logData.type == Raumkernel::Log::LogType::LOGTYPE_CRITICALERROR ||
        _logData.type == Raumkernel::Log::LogType::LOGTYPE_ERROR)
        logTypeWord = EVENTLOG_ERROR_TYPE;
    if (_logData.type == Raumkernel::Log::LogType::LOGTYPE_WARNING)
        logTypeWord = EVENTLOG_WARNING_TYPE;

    WriteEventLogEntry(_logData.log, logTypeWord);
}