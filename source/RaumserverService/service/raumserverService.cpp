
#include "service/raumserverService.h"


RaumserverService::RaumserverService(std::string _serviceName, bool _canStop, bool _canShutdown, bool _canPauseContinue)
: CServiceBase(_serviceName, _canStop, _canShutdown, _canPauseContinue)
{
    isStopping = false;

    // Create a manual-reset event that is not signaled at first to indicate 
    // the stopped signal of the service.
    stoppedEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (stoppedEventHandle == NULL)
    {
        writeEventLogEntry("Raumserver Service creation error!", EVENTLOG_ERROR_TYPE);
        throw GetLastError();
    }
}


RaumserverService::~RaumserverService(void)
{
    if (stoppedEventHandle)
    {
        CloseHandle(stoppedEventHandle);
        stoppedEventHandle = NULL;
    }
}


void RaumserverService::onStart(DWORD dwArgc, LPWSTR *lpszArgv)
{
    // Log a service start message to the Application log.
    writeEventLogEntry("Raumserver service in OnStart", EVENTLOG_INFORMATION_TYPE);
    // Queue the main service function for execution in a worker thread.
    CThreadPool::QueueUserWorkItem(&RaumserverService::serviceWorkerThread, this);
}


void RaumserverService::serviceWorkerThread(void)
{    
    writeEventLogEntry("Raumserver worker thread started", EVENTLOG_INFORMATION_TYPE);
   
    std::string executionPath = getExecutionPath();
    writeEventLogEntry("Raumserver working directory is: " + executionPath, EVENTLOG_INFORMATION_TYPE);
    auto settingsFile = executionPath + "\\raumserver.xml";
    auto logFileDirectory = executionPath + "\\logs\\";

    writeEventLogEntry("Raumserver settings file is: " + settingsFile, EVENTLOG_INFORMATION_TYPE);
    writeEventLogEntry("Raumserver log file directory is: " + logFileDirectory, EVENTLOG_INFORMATION_TYPE);

    // create raumserver object adn do init init
    raumserverObject = std::unique_ptr<Raumserver::Raumserver>(new Raumserver::Raumserver());        
    raumserverObject->setSettingsFile(settingsFile);
    raumserverObject->setLogFilePath(logFileDirectory);
    
    connections.connect(raumserverObject->getLogObject()->sigLog, this, &RaumserverService::onLog);

    raumserverObject->init();
   

    // Periodically check if the service is stopping.
    while (!isStopping)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));      
    }

    // Signal the stopped event.
    SetEvent(stoppedEventHandle);
}


void RaumserverService::onStop()
{
    // Log a service stop message to the Application log.
    writeEventLogEntry("Raumserver service in OnStop", EVENTLOG_INFORMATION_TYPE);

    // Indicate that the service is stopping and wait for the finish of the 
    // main service function (ServiceWorkerThread).
    isStopping = TRUE;
    if (WaitForSingleObject(stoppedEventHandle, INFINITE) != WAIT_OBJECT_0)
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

    writeEventLogEntry(_logData.log, logTypeWord);
}