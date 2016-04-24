
#include "raumserverInstallerView.h"


//http://sciter.com/developers/embedding-principles/
//http://www.terrainformatica.com/2012/11/sciter-ui-application-architecture/


ApplicationWindow::ApplicationWindow() : window(SW_MAIN | SW_ALPHA | SW_POPUP | SW_ENABLE_DEBUG, wrc)
{    
}


void ApplicationWindow::init()
{
    raumserverInstallerObject.init();
    raumserverInstallerObject.initDiscover();

    connections.connect(raumserverInstallerObject.sigDeviceFoundForInstall, this, &ApplicationWindow::onDeviceFoundForInstall);
    connections.connect(raumserverInstallerObject.sigDeviceRemovedForInstall, this, &ApplicationWindow::onDeviceRemovedForInstall);
    connections.connect(raumserverInstallerObject.sigDeviceInformationChanged, this, &ApplicationWindow::onDeviceInformationChanged);
    connections.connect(raumserverInstallerObject.sigInstallProgressInformation, this, &ApplicationWindow::onInstallProgressInformation);
    connections.connect(raumserverInstallerObject.sigInstallCompleted, this, &ApplicationWindow::onInstallCompleted);
}


sciter::value ApplicationWindow::getNetworkAdapterInformation()
{
    Json::Value root, networkAdapter;
    auto adapterInfoList = raumserverInstallerObject.getNetworkAdapterList();

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
        raumserverInstallerObject.setNetworkAdapter(raumserverInstallerObject.getNetworkAdapterInformation(adapterId));
    }
    
    return true;
}


sciter::value ApplicationWindow::startSearchingForDevices()
{
    raumserverInstallerObject.startDiscoverDevicesForInstall();  
    // TODO: Start a timer which update a progress bar and will run about 20 seconds
    // if nothing is found in 20 seconds then give some advice to the user
    return true;
}


sciter::value ApplicationWindow::startInstallOnDevice(sciter::value _ip)
{        
    std::wstring ip = std::wstring(_ip.to_string().c_str());
    auto deviceInfo = raumserverInstallerObject.getDeviceInformation(Converter::wstring2string(ip));
    if (deviceInfo.ip.empty())
        return false;    
    raumserverInstallerObject.startInstallToDevice(deviceInfo);
    return true;
}


sciter::value ApplicationWindow::startRemoveFromDevice(sciter::value _ip)
{    
    // TODO: 
    return true;
}


void ApplicationWindow::onDeviceFoundForInstall(RaumserverInstaller::DeviceInformation _deviceInfo)
{
    Json::Value deviceInfo;

    std::unique_lock<std::mutex> lock(lockDeviceAction);

    
    
    // TODO: @@ amke a method for this!
    // convert deviceInfo to JSON string 
    deviceInfo["deviceInfo"]["ip"] = _deviceInfo.ip;
    deviceInfo["deviceInfo"]["name"] = _deviceInfo.name;
    deviceInfo["deviceInfo"]["udn"] = _deviceInfo.UDN;
    deviceInfo["deviceInfo"]["sshAccess"] = _deviceInfo.sshAccess;
    deviceInfo["deviceInfo"]["raumserverInstalled"] = _deviceInfo.raumserverInstalled;
    deviceInfo["deviceInfo"]["raumserverRuns"] = _deviceInfo.raumserverRuns;
    deviceInfo["deviceInfo"]["raumserverVersion"] = _deviceInfo.raumserverVersion;
    deviceInfo["deviceInfo"]["type"] = _deviceInfo.type;

    call_function("DeviceSelection.addDeviceInfo", sciter::value(_deviceInfo.ip), sciter::value(deviceInfo.toStyledString()));                                                           
}


void ApplicationWindow::onDeviceRemovedForInstall(RaumserverInstaller::DeviceInformation _deviceInfo)
{
    std::unique_lock<std::mutex> lock(lockDeviceAction);
}


void ApplicationWindow::onDeviceInformationChanged(RaumserverInstaller::DeviceInformation _deviceInfo)
{
    std::unique_lock<std::mutex> lock(lockDeviceAction);
    call_function("DeviceSelection.updateDeviceInfo", sciter::value(_deviceInfo.ip), sciter::value(_deviceInfo.getJsonValue().toStyledString()));
}


void ApplicationWindow::onInstallProgressInformation(RaumserverInstaller::DeviceInstaller::DeviceInstallerProgressInfo _progressInfo)
{
    call_function("InstallProgressPage.addProgressInfo", sciter::value(_progressInfo.getJsonValue().toStyledString()));
}


void ApplicationWindow::onInstallCompleted(RaumserverInstaller::DeviceInstaller::DeviceInstallerProgressInfo _progressInfo)
{
    call_function("InstallProgressPage.installationDone", sciter::value(_progressInfo.getJsonValue().toStyledString()));
}


