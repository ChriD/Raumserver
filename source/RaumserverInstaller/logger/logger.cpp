
#include <logger/logger.h>


namespace Raumkernel
{
    namespace Log
    {
        Log::Log()
        {
            // as standard value we set the log level to 'LOGDEBUG'
            logTypeLevel = LogType::LOGTYPE_DEBUG;
        }


        Log::~Log()
        {
        }


        // We pass the logAdapter shared_ptr per value. This code is non performance relevant and therfore we do not need to pass it by reference
        // (but of course this would be ok in this case because "pushing back" to the vector would create a copy again and so the usage count would 
        // never fall down to 0 and we keep the vector elements alive as long as the class is existent!)
        void Log::registerAdapter(std::shared_ptr<LogAdapter> _logAdapter)
        {
            std::unique_lock<std::mutex> lock(mutexLog);
            logAdapterList.push_back(_logAdapter);
        }


        void Log::debug(const std::string &_log, const std::string &_location)
        {
            addLog(LogType::LOGTYPE_DEBUG, _log, _location);
        }


        void Log::warning(const std::string &_log, const std::string &_location)
        {
            addLog(LogType::LOGTYPE_WARNING, _log, _location);
        }


        void Log::info(const std::string &_log, const std::string &_location)
        {
            addLog(LogType::LOGTYPE_INFO, _log, _location);
        }


        void Log::error(const std::string &_log, const std::string &_location)
        {
            addLog(LogType::LOGTYPE_ERROR, _log, _location);
        }


        void Log::critical(const std::string &_log, const std::string &_location)
        {
            addLog(LogType::LOGTYPE_CRITICALERROR, _log, _location);
        }


        void Log::setLogLevel(LogType _logTypeLevel)
        {
            std::unique_lock<std::mutex> lock(mutexLog);
            logTypeLevel = _logTypeLevel;
        }


        void Log::addLog(LogType _logType, const  std::string &_log, const std::string &_location)
        {
            // check log level of log so we can decide if we should log or not
            if (_logType > logTypeLevel)
                return;

            mutexLog.lock();

            // fill log data struct with the available log data and the current timestamp
            // this struct is used by the adapters and as parameter for signal subsribers
            LogData logData;
            logData.type = _logType;
            logData.log = _log;
            logData.location = _location;
            logData.logDateTimeStamp = Raumkernel::Tools::DateUtil::getCurrentDateTimeStamp();

            // run through all defined adapters (shared pointers) and call 'log' on them 
            // we will not catch any critical errors here, this has to be done by the adapter if necessary!
            for (auto it = logAdapterList.begin(); it != logAdapterList.end(); it++)
            {
                try
                {
                    it->get()->log(logData);
                }
                // catch all the stuff the attached adapters may throw, those may be some standard exception or an RaumkernelException 
                // its a pitty that the exception occoured in the log class itself, so we are not able to log this exception.
                catch (Raumkernel::Exception::RaumkernelException &e)
                {
                    if (e.type() == Raumkernel::Exception::ExceptionType::EXCEPTIONTYPE_APPCRASH)
                        throw e;
                    // if the raumkenrel exception is recoverable (no appcrash) we so nothing. We can not log any error because the error happend in 
                    // the log object.        
                }
                catch (std::exception &e)
                {
                    throw e;
                }
                catch (std::string &e)
                {
                    throw e;
                }
                catch (...)
                {
                    throw std::runtime_error("Unknown exception!");
                }
            }

            mutexLog.unlock();

            // after we have release the lock we can signal attached subscribers
            // we do not want the subscriber to be in the same mutex lock. The subscriber should handle this by himself            
            sigLog.fire(logData);
        }


        LogType Log::logTypeStringToLogType(std::string _logTypeString)
        {
            if (_logTypeString == "DEBUG") return LogType::LOGTYPE_DEBUG;
            if (_logTypeString == "ERROR") return LogType::LOGTYPE_ERROR;
            if (_logTypeString == "INFO") return LogType::LOGTYPE_INFO;
            if (_logTypeString == "WARNING") return LogType::LOGTYPE_WARNING;
            if (_logTypeString == "CRITICAL") return LogType::LOGTYPE_CRITICALERROR;
            return LogType::LOGTYPE_DEBUG;
        }
    }
}