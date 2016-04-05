
#include <logger/loggerAdapter.h>


namespace Raumkernel
{
    namespace Log
    {
        LogAdapter::LogAdapter()
        {
        }


        LogAdapter::~LogAdapter()
        {
        }


        void LogAdapter::log(LogData _logData)
        {
        }


        std::string LogAdapter::getLogTypeString(LogType _logType)
        {
            switch (_logType)
            {
            case LogType::LOGTYPE_CRITICALERROR:
                return "CRITICAL";
            case LogType::LOGTYPE_ERROR:
                return "ERROR";
            case LogType::LOGTYPE_WARNING:
                return "WARNING";
            case LogType::LOGTYPE_INFO:
                return "INFO";
            case LogType::LOGTYPE_DEBUG:
                return "DEBUG";
            default:
                break;
            }
            return "UNKNOWN";
        }


        std::string LogAdapter::getFormattedCurrentDateTimeString(Raumkernel::Tools::DateTimeStamp _stamp)
        {
            std::string milisecondsString = std::to_string(_stamp.millisecond);
            return  std::to_string(_stamp.year) + "." +
                Raumkernel::Tools::StringUtil::padL(std::to_string(_stamp.month), 2, '0') + "." +
                Raumkernel::Tools::StringUtil::padL(std::to_string(_stamp.day), 2, '0') + " " +
                Raumkernel::Tools::StringUtil::padL(std::to_string(_stamp.hour), 2, '0') + ":" +
                Raumkernel::Tools::StringUtil::padL(std::to_string(_stamp.minute), 2, '0') + ":" +
                Raumkernel::Tools::StringUtil::padL(std::to_string(_stamp.second), 2, '0') + "." +
                Raumkernel::Tools::StringUtil::padL(std::to_string(_stamp.millisecond), 3, '0');
        }



        LogAdapter_File::LogAdapter_File() : LogAdapter()
        {
            // specify default log file path
            logFilePath = "logs/";
            // generate a log file name with date and time so each creation (startup?) of the logger object will result in an own log file
            logFileName = getCurrentDateTimeFileString() + ".log";
        }


        LogAdapter_File::~LogAdapter_File()
        {
        }


        void LogAdapter_File::setLogFilePath(const std::string &_path)
        {
            logFilePath = _path;
        }


        void LogAdapter_File::log(LogData _logData)
        {
            std::string logString = getFormattedCurrentDateTimeString(_logData.logDateTimeStamp) + " " + Raumkernel::Tools::StringUtil::padR(getLogTypeString(_logData.type) + ":", 10) + _logData.log + " [" + _logData.location + "]";

            // write log into logFile. we do open and close the file each time we write a log. This is a little bit slower than keeping the handle open (about 0.3ms per log entry)  
            // But we will be sure the log file is not locked in the worst case of an unexpected application shutdown.            
            std::ofstream logFile;
            logFile.open(logFilePath + logFileName, std::fstream::out | std::fstream::app);
            if (logFile.is_open())
            {
                logFile << logString << std::endl;
                logFile.flush();
                logFile.close();
            }
            else
            {
                // we throw an exception if we can not open the log file, but we throw it of type recoverable, that means the app won't crash in this situation
                throw Raumkernel::Exception::RaumkernelException(Raumkernel::Exception::ExceptionType::EXCEPTIONTYPE_RECOVERABLE, CURRENT_POSITION, "File '" + logFilePath + logFileName + "' could not be created");
            }
        }


        std::string LogAdapter_File::getCurrentDateTimeFileString()
        {
            Raumkernel::Tools::DateTimeStamp stamp = Raumkernel::Tools::DateUtil::getCurrentDateTimeStamp();
            return  std::to_string(stamp.year) +
                Raumkernel::Tools::StringUtil::padL(std::to_string(stamp.month), 2, '0') +
                Raumkernel::Tools::StringUtil::padL(std::to_string(stamp.day), 2, '0') +
                Raumkernel::Tools::StringUtil::padL(std::to_string(stamp.hour), 2, '0') +
                Raumkernel::Tools::StringUtil::padL(std::to_string(stamp.minute), 2, '0') +
                Raumkernel::Tools::StringUtil::padL(std::to_string(stamp.second), 2, '0') +
                Raumkernel::Tools::StringUtil::padL(std::to_string(stamp.millisecond), 3, '0');
        }




        LogAdapter_Console::LogAdapter_Console() : LogAdapter()
        {
        }


        LogAdapter_Console::~LogAdapter_Console()
        {
        }


        void LogAdapter_Console::log(LogData _logData)
        {
            std::string logString = getFormattedCurrentDateTimeString(_logData.logDateTimeStamp) + " " + Raumkernel::Tools::StringUtil::padR(getLogTypeString(_logData.type) + ":", 10) + _logData.log;

            // output of log to console (here we are limited to 79 on windows, linux provides more cols)    
            if (CONSOLE_COLS > 0)
                logString.resize(CONSOLE_COLS);
            std::cout << logString << std::endl;
        }
    }
}