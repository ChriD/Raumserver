#include "raumserverInstallerView.h"
#include "versionNumberInstaller.h"


ApplicationWindow::ApplicationWindow() : window(SW_MAIN | SW_ALPHA | SW_POPUP | SW_ENABLE_DEBUG, wrc)
{
    raumserverInstallerObject = nullptr;

    settingsFileName = "settings.xml";

    versionInfoApp.appName = AppNameInstaller;
    versionInfoApp.appVersion = AppVersionNumberInstaller;
    versionInfoApp.appVersionBuild = AppVersionBuildInstaller;
    versionInfoApp.appVersionName = AppVersionNameInstaller;      


    #ifdef _WIN32
    LPCWSTR icon = L"icon.ico";    
    HICON hIcon = (HICON)LoadImage(NULL, icon, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    SendMessage(get_hwnd(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    #endif

    #ifdef __linux__ 
    // linux does not have a window icon. It uses a .desktop file instead to show a icon in a menu or on desktop    
    #endif

}


void ApplicationWindow::init()
{       
    raumserverInstallerObject = new RaumserverInstaller::RaumserverInstaller();

    raumserverInstallerObject->init();
    raumserverInstallerObject->initLogObject(Log::LogType::LOGTYPE_INFO);    
    raumserverInstallerObject->initDiscover();

    settingsManager.setFileName(settingsFileName);
    settingsManager.setLogObject(raumserverInstallerObject->getLogObject());       
    settingsManager.initSettings();
        
    currentVersionInfoWebUrl = settingsManager.getValue(".//RaumserverInstaller//currentVersion");
    #ifdef __linux__ 
        currentVersionBinarySource = settingsManager.getValue(".//RaumserverInstaller//binarySource[@type='linux']");
    #elif _WIN32
        currentVersionBinarySource = settingsManager.getValue(".//RaumserverInstaller//binarySource[@type='windows']");
    #else
        std::runtime_error("");
    #endif    
    currentServerVersionInfoWebUrl = settingsManager.getValue(".//RaumserverInstaller//currentServerVersion");        

    versionInfoLib = raumserverInstallerObject->getVersionInfo();    
    versionInfoServer.loadFromXMLFile("binaries/version.xml");

    
    connections.connect(raumserverInstallerObject->sigDeviceFoundForInstall, this, &ApplicationWindow::onDeviceFoundForInstall);
    connections.connect(raumserverInstallerObject->sigDeviceRemovedForInstall, this, &ApplicationWindow::onDeviceRemovedForInstall);
    connections.connect(raumserverInstallerObject->sigDeviceInformationChanged, this, &ApplicationWindow::onDeviceInformationChanged);
    connections.connect(raumserverInstallerObject->sigInstallProgressInformation, this, &ApplicationWindow::onInstallProgressInformation);
    connections.connect(raumserverInstallerObject->sigInstallCompleted, this, &ApplicationWindow::onInstallCompleted);
    
    checkForNewVersion();    
    checkForNewServerVersion();   
}


void ApplicationWindow::checkForNewVersion()
{      
    checkForNewVersionThreadObject = std::thread(&ApplicationWindow::checkForNewVersionThread, this);
}


void ApplicationWindow::checkForNewVersionThread()
{
    VersionInfo::VersionInfo newestVersion, noVersion;
    try
    {        
        noVersion.clear();
        newestVersion.loadFromUrl(currentVersionInfoWebUrl);
        onCheckForNewVersionResult(newestVersion);
    }
    catch (...)
    {
        onCheckForNewVersionResult(noVersion);
    }
}


void ApplicationWindow::onCheckForNewVersionResult(VersionInfo::VersionInfo _versioninfo)
{        
    if (_versioninfo.appVersionBuild == 0)
    {
        // no connect to update server...
        std::unique_lock<std::mutex> lock(lockGuiUpdate);
        call_function("Application.newVersionCheckFailed");
    }
    else if (_versioninfo.appVersionBuild > versionInfoApp.appVersionBuild)
    {        
        // new version is ready for download
        std::unique_lock<std::mutex> lock(lockGuiUpdate);
        call_function("Application.newVersionAvailable", sciter::value(currentVersionBinarySource), sciter::value(_versioninfo.appVersion), sciter::value(std::to_string(_versioninfo.appVersionBuild)));
    }
    else
    {
        call_function("Application.versionIsUpToDate", sciter::value(_versioninfo.appVersion), sciter::value(std::to_string(_versioninfo.appVersionBuild)));
    }
}


void ApplicationWindow::checkForNewServerVersion()
{
    checkForNewServerVersionThreadObject = std::thread(&ApplicationWindow::checkForNewServerVersionThread, this);
}


void ApplicationWindow::checkForNewServerVersionThread()
{
    VersionInfo::VersionInfo newestVersion, noVersion;
    try
    {
        noVersion.clear();
        newestVersion.loadFromUrl(currentServerVersionInfoWebUrl);
        onCheckForNewServerVersionResult(newestVersion);
    }
    catch (...)
    {
        onCheckForNewServerVersionResult(noVersion);
    }
}


void ApplicationWindow::onCheckForNewServerVersionResult(VersionInfo::VersionInfo _versioninfo)
{
    if (_versioninfo.appVersionBuild == 0)
    {
        // no connect to update server...
        std::unique_lock<std::mutex> lock(lockGuiUpdate);
        call_function("Application.newServerVersionCheckFailed");
    }
    else if (_versioninfo.appVersionBuild > versionInfoServer.appVersionBuild)
    {
        // new version is ready for download
        std::unique_lock<std::mutex> lock(lockGuiUpdate);
        call_function("Application.newServerVersionAvailable", sciter::value(_versioninfo.appVersion), sciter::value(std::to_string(_versioninfo.appVersionBuild)));;
    }
    else
    {
        call_function("Application.serverVersionIsUpToDate", sciter::value(_versioninfo.appVersion), sciter::value(std::to_string(_versioninfo.appVersionBuild)));;
    }
}


sciter::value ApplicationWindow::appClosing()
{
    // wait till the version checkers are done
    if (checkForNewVersionThreadObject.joinable())
        checkForNewVersionThreadObject.join();
    if (checkForNewServerVersionThreadObject.joinable())
        checkForNewServerVersionThreadObject.join();

    // disconnect all signals
    connections.disconnect_all(true);

    // destroy the installer object
    if (raumserverInstallerObject)
        delete raumserverInstallerObject;

    return true;
}


sciter::value ApplicationWindow::getInstallerVersionInfo()
{    
    Json::Value root, versionInfoLibNode, versionInfoAppNode;

    versionInfoAppNode["app"]["name"] = versionInfoApp.appName;
    versionInfoAppNode["app"]["version"] = versionInfoApp.appVersion;
    versionInfoAppNode["app"]["versionName"] = versionInfoApp.appVersionName;
    versionInfoAppNode["app"]["build"] = versionInfoApp.appVersionBuild;
    root["version"].append(versionInfoAppNode);

    versionInfoLibNode["lib"]["name"] = versionInfoLib.appName;
    versionInfoLibNode["lib"]["version"] = versionInfoLib.appVersion;
    versionInfoLibNode["lib"]["versionName"] = versionInfoLib.appVersionName;
    versionInfoLibNode["lib"]["build"] = versionInfoLib.appVersionBuild;
    root["version"].append(versionInfoLibNode);

    return root.toStyledString();      
}


sciter::value ApplicationWindow::getNetworkAdapterInformation()
{
    Json::Value root, networkAdapter;

    auto adapterInfoList = raumserverInstallerObject->getNetworkAdapterList();

    for (auto adapterInfo : adapterInfoList)
    {
        networkAdapter["networkAdapter"]["name"] = adapterInfo.name;
        networkAdapter["networkAdapter"]["address"] = adapterInfo.address;
        networkAdapter["networkAdapter"]["id"] = adapterInfo.id;
        root["networkAdapterInformations"].append(networkAdapter);
    }    

    if (!adapterInfoList.size())
    {
        networkAdapter["networkAdapter"]["name"] = "No network adapter available";
        networkAdapter["networkAdapter"]["address"] = "";
        networkAdapter["networkAdapter"]["id"] = 0;
        root["networkAdapterInformations"].append(networkAdapter);
    }

    return root.toStyledString();
}


sciter::value ApplicationWindow::selectNetworkAdapter(sciter::value _adapterId)
{
    std::uint16_t adapterId = _adapterId.get(0);
    if (adapterId > 0)
    {
        raumserverInstallerObject->setNetworkAdapter(raumserverInstallerObject->getNetworkAdapterInformation(adapterId));
    }

    return true;
}


sciter::value ApplicationWindow::startSearchingForDevices()
{
    raumserverInstallerObject->startDiscoverDevicesForInstall();
    return true;
}


sciter::value ApplicationWindow::startInstallOnDevice(sciter::value _ip)
{    
    sciter::string ip = _ip.to_string();
    std::string ipUni = w2u(ip);     

    auto deviceInfo = raumserverInstallerObject->getDeviceInformation(ipUni);
    if (deviceInfo.ip.empty())
        return false;
    raumserverInstallerObject->startInstallToDevice(deviceInfo);
    
    return true;
}


sciter::value ApplicationWindow::startRemoveFromDevice(sciter::value _ip)
{
    sciter::string ip = _ip.to_string();
    std::string ipUni = w2u(ip);
    
    auto deviceInfo = raumserverInstallerObject->getDeviceInformation(ipUni);
    if (deviceInfo.ip.empty())
        return false;
    raumserverInstallerObject->startRemoveFromDevice(deviceInfo);

    return true;
}


void ApplicationWindow::onDeviceFoundForInstall(RaumserverInstaller::DeviceInformation _deviceInfo)
{    
    std::unique_lock<std::mutex> lock(lockGuiUpdate);
    call_function("DeviceSelection.addDeviceInfo", sciter::value(_deviceInfo.ip), sciter::value(_deviceInfo.getJsonValue().toStyledString()));
}


void ApplicationWindow::onDeviceRemovedForInstall(RaumserverInstaller::DeviceInformation _deviceInfo)
{    
    std::unique_lock<std::mutex> lock(lockGuiUpdate);
    call_function("DeviceSelection.removeDeviceInfo", sciter::value(_deviceInfo.ip), sciter::value(_deviceInfo.getJsonValue().toStyledString()));
}


void ApplicationWindow::onDeviceInformationChanged(RaumserverInstaller::DeviceInformation _deviceInfo)
{    
    std::unique_lock<std::mutex> lock(lockGuiUpdate);
    call_function("DeviceSelection.updateDeviceInfo", sciter::value(_deviceInfo.ip), sciter::value(_deviceInfo.getJsonValue().toStyledString()));
}


void ApplicationWindow::onInstallProgressInformation(RaumserverInstaller::DeviceInstaller::DeviceInstallerProgressInfo _progressInfo)
{    
    std::unique_lock<std::mutex> lock(lockGuiUpdate);    
    call_function("InstallProgressPage.addProgressInfo", sciter::value(_progressInfo.getJsonValue().toStyledString()));
}


void ApplicationWindow::onInstallCompleted(RaumserverInstaller::DeviceInstaller::DeviceInstallerProgressInfo _progressInfo)
{    
    std::unique_lock<std::mutex> lock(lockGuiUpdate);   
    call_function("InstallProgressPage.installationDone", sciter::value(_progressInfo.getJsonValue().toStyledString()));
}


