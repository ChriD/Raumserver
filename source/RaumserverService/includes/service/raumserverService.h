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
#ifndef RAUMSERVER_WINDOWSSERVICE_H
#define RAUMSERVER_WINDOWSSERVICE_H

#include <Windows.h>
#include "service/serviceBase.h"
#include "raumserver/raumserver.h"
#include "tools/threadPool.h"

class RaumserverService : public CServiceBase
{
    public:

        RaumserverService(std::string _serviceName, bool _canStop = true,  bool _canShutdown = true,  bool _canPauseContinue = false);
        virtual ~RaumserverService(void);

    protected:               
        /**
        * The function is executed when a Start command is sent to the 
        * service by the SCM or when the operating system starts (for a service
        * that starts automatically). It specifies actions to take when the  service starts
        * method blocks until the service has stopped.
        */
        virtual void onStart(DWORD _dwArgc, PWSTR *_pszArgv) override;            
        /**
        * The function is executed when a Stop command is sent to the
        * service by SCM. It specifies actions to take when a service stops 
        * running. In this code sample, OnStop logs a service-stop message to the
        * Application log, and waits for the finish of the main service function.
        */
        virtual void onStop() override;
        /**
        * Will be called when a log on the Raumkernel or Raumserver is logged
        * It used to write the log to the eventViewer (At least some Errors)
        */
        virtual void onLog(Raumkernel::Log::LogData _logData);        
        /**
        * The method performs the main function of the service. It runs 
        *  on a thread pool worker thread.
        */
        void serviceWorkerThread(void);

    private:

        /**
        * The Raumserver Object (which includes the Raumkernel)
        */
        std::unique_ptr<Raumserver::Raumserver> raumserverObject;
        /**
        * Indicates that the Service is in stopping mode
        */
        bool isStopping;
        HANDLE stoppedEventHandle;

        sigs::connections connections;
};


#endif