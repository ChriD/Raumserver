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
#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include <windows.h>
#include <string>


class CServiceBase
{
    public:         
        /**
        * Service object constructor. The optional parameters (_canStop
        * _canShutdown and _canPauseContinue) allow you to specify whether the 
        * service can be stopped, paused and continued, or be notified when
        * system shutdown occurs.
        */
        CServiceBase(std::string _serviceName, bool _canStop = true, bool _canShutdown = true, bool _canPauseContinue = false);
        /**
        * Service object destructor
        */
        virtual ~CServiceBase(void);
        /**
        * Stops the service
        */
        void stopService();
        /**
        * Log a message to the Application event log.
        */
        void writeEventLogEntry(std::string _message, WORD _type); 
        /**
        * Log an error message to the Application event log.
        */
        void writeErrorLogEntry(std::string _function, DWORD _error = GetLastError());
        /**
        * Register the executable for a service with the Service Control Manager
        * (SCM). After you call Run(ServiceBase), the SCM issues a Start command,
        * which results in a call to the OnStart method in the service. This
        * method blocks until the service has stopped.
        */
        static BOOL run(CServiceBase &service);

    protected:

        /**
        * When implemented in a derived class, executes when a Start command is 
        * sent to the service by the SCM or when the operating system starts 
        * (for a service that starts automatically). Specifies actions to take 
        * when the service starts.
        */
        virtual void onStart(DWORD _argc, PWSTR *_argv);
        /**
        * When implemented in a derived class, executes when a Stop command is 
        * sent to the service by the SCM. Specifies actions to take when a 
        * service stops running.
        */
        virtual void onStop();
        /* When implemented in a derived class, executes when a Pause command is 
        * sent to the service by the SCM. Specifies actions to take when a 
        * service pauses.
        */
        virtual void onPause();
        /**
        * When implemented in a derived class, OnContinue runs when a Continue 
        * command is sent to the service by the SCM. Specifies actions to take 
        * when a service resumes normal functioning after being paused.
        */
        virtual void onContinue();
        /**
        * When implemented in a derived class, executes when the system is 
        * shutting down. Specifies what should occur immediately prior to the 
        * system shutting down.
        */
        virtual void onShutdown();
        /**
        *  Returns the execution path of the executable (service)
        */
        virtual std::string getExecutionPath();
        /**
        *  Set the service status and report the status to the SCM.
        */
        void SetServiceStatus(DWORD _currentState, DWORD _win32ExitCode = NO_ERROR, DWORD _waitHint = 0);    

    private:
        /**
        *  Entry point for the service. It registers the handler function for the 
        * service and starts the service.
        */
        static void WINAPI ServiceMain(DWORD _argc, LPWSTR *_argv);
        /**
        *  The function is called by the SCM whenever a control code is sent to the service.
        */
        static void WINAPI ServiceCtrlHandler(DWORD _ctrl);
        /**
        * Start the service.
        */
        void startService(DWORD _argc, PWSTR *_argv);
        /**
        * Pause the service
        */
        void pauseService();
        /**
        * Resume the service after being paused.
        */
        void continueService();        
        /**
        * Execute when the system is shutting down.
        */
        void shutdownService();        
        /**
        * The singleton service instance.
        */
        static CServiceBase *service;        
        /**
        * The name of the service
        */
        std::string name;
        /**
        * The status of the service
        */
        SERVICE_STATUS status;        
        /**
        * The service status handle
        */
        SERVICE_STATUS_HANDLE statusHandle;
};


#endif