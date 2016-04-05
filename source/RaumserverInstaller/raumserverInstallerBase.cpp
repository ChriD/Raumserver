
#include "raumserverInstallerBase.h"

namespace RaumkernelInstaller
{

    RaumserverInstallerBase::RaumserverInstallerBase()
    {
    }


    RaumserverInstallerBase::~RaumserverInstallerBase()
    {
    }


    void RaumserverInstallerBase::setLogObject(std::shared_ptr<Raumkernel::Log::Log> _logObject)
    {
        logObject = _logObject;
    }


    std::shared_ptr<Raumkernel::Log::Log> RaumserverInstallerBase::getLogObject()
    {
        return logObject;
    }


    void RaumserverInstallerBase::logDebug(const std::string &_log, const std::string &_location)
    {
        if (logObject == nullptr)
            return;
        logObject->debug(_log, _location);
    }


    void RaumserverInstallerBase::logWarning(const std::string &_log, const std::string &_location)
    {
        if (logObject == nullptr)
            return;
        logObject->warning(_log, _location);
    }


    void RaumserverInstallerBase::logInfo(const std::string &_log, const std::string &_location)
    {
        if (logObject == nullptr)
            return;
        logObject->info(_log, _location);
    }


    void RaumserverInstallerBase::logError(const std::string &_log, const std::string &_location)
    {
        if (logObject == nullptr)
            return;
        logObject->error(_log, _location);
    }


    void RaumserverInstallerBase::logCritical(const std::string &_log, const std::string &_location)
    {
        if (logObject == nullptr)
            return;
        logObject->critical(_log, _location);
    }

}

