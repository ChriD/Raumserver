
#include "raumserverInstallerView.h"


//http://sciter.com/developers/embedding-principles/
//http://www.terrainformatica.com/2012/11/sciter-ui-application-architecture/


frame::frame() : window(SW_MAIN | SW_ALPHA | SW_POPUP | SW_ENABLE_DEBUG, wrc) 
{
    //raumfeldDeviceFinder.init();
    //raumfeldDeviceFinder.loadNetworkAdaptersInformation();
}


sciter::value frame::getNetworkAdapterInformation()
{
    /*
    Json::Value root, networkAdapter;
    auto adaptersInfo = raumfeldDeviceFinder.getNetworkAdaptersInformation();

    for (auto adapterInfo : adaptersInfo)
    {
        networkAdapter["networkAdapter"]["name"] = adapterInfo.name;
        networkAdapter["networkAdapter"]["address"] = adapterInfo.address;
        networkAdapter["networkAdapter"]["id"] = adapterInfo.id;
        root["networkAdapterInformations"].append(networkAdapter);
    }

    if (!adaptersInfo.size())
    {
        networkAdapter["networkAdapter"]["name"] = "No network adapter available";
        networkAdapter["networkAdapter"]["address"] = "";
        networkAdapter["networkAdapter"]["id"] = 0;
        root["networkAdapterInformations"].append(networkAdapter);
    }
    
    return root.toStyledString();    
    */
    return "";
}


sciter::value frame::selectNetworkAdapter(sciter::value _adapterId)
{            

    //raumfeldDeviceFinder.selectAdapterId(_adapterId.get(0));
    return true;
}


sciter::value frame::startSearchingForDevices()
{

    //raumfeldDeviceFinder.startSearchingForDevices();
    return true;
}


/*
sciter::value frame::testCpp(json::value param1, json::value param2)
{
    //window* pw = ...    
    this->call_function("NetworkAdapterSelection.addNetworkAdapter", "Test1");
    this->call_function("NetworkAdapterSelection.addNetworkAdapter", "Test2");    


    return sciter::value(1);
}
*/
