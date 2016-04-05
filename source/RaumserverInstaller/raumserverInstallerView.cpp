
#include "raumserverInstallerView.h"


//http://sciter.com/developers/embedding-principles/
//http://www.terrainformatica.com/2012/11/sciter-ui-application-architecture/

sciter::value frame::getNetworkAdapterInformation()
{
    //json::value returnValue;    

    Json::Value root, networkAdapter;
    //std::string my_encoding = root.get("my-encoding", "UTF-32").asString();


   // const Json::Value my_plugins = root["my-plug-ins"];
    //for (int index = 0; index < my_plugins.size(); ++index)  // Iterates over the sequence elements.
    //    yourlib::loadPlugIn(my_plugins[index].asString());

 //   yourlib::setIndentLength(root["my-indent"].get("length", 3).asInt());
 //   yourlib::setIndentUseSpace(root["my-indent"].get("use_space", true).asBool());

    // ...
    // At application shutdown to make the new configuration document:
    // Since Json::Value has implicit constructor for all value types, it is not
    // necessary to explicitly construct the Json::Value object:
    //root["networkAdapterInformations"

    networkAdapter["networkAdapter"]["name"] = "Adapter 1";
    networkAdapter["networkAdapter"]["SubnetMask"] = "255.0.0.0";
    root["networkAdapterInformations"].append(networkAdapter);

    networkAdapter["networkAdapter"]["name"] = "Adapter 2";
    networkAdapter["networkAdapter"]["SubnetMask"] = "255.0.0.0";
    root["networkAdapterInformations"].append(networkAdapter);

    
    //returnValue.set_item("Name", sciter::value("Test1"));
    //returnValue.set_item("SubnetMask:", sciter::value("255.0.0.0"));

    auto test = root.toStyledString();

    std::string str = root.toStyledString();

    //j//son::value


    return sciter::value(root.toStyledString());
}


sciter::value frame::testCpp(json::value param1, json::value param2)
{
    //window* pw = ...    
    this->call_function("NetworkAdapterSelection.addNetworkAdapter", "Test1");
    this->call_function("NetworkAdapterSelection.addNetworkAdapter", "Test2");    

    /*
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof (WINDOWPLACEMENT);
    //GetWindowPlacement(Sess.M_hdl, &wp);
    const sciter::value arr[4] = { wp.rcNormalPosition.left, wp.rcNormalPosition.top, wp.rcNormalPosition.right - wp.rcNormalPosition.left, wp.rcNormalPosition.bottom - wp.rcNormalPosition.top };
    return sciter::value(arr);
    */
    return sciter::value(1);
}
