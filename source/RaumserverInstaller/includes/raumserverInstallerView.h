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
#ifndef RAUMSERVERINSTALLER_VIEW_H
#define RAUMSERVERINSTALLER_VIEW_H

#include <raumserverInstaller.h>
#include <signals/signals.hpp>
#include <json/json.h>
#include "sciter-x-window.hpp"

static RECT wrc = { 100, 100, 600, 400 };

class ApplicationWindow : public sciter::window
{
    public:
        ApplicationWindow();
        void init();

        sciter::value getNetworkAdapterInformation();
        sciter::value selectNetworkAdapter(sciter::value _adapterId);
        sciter::value startSearchingForDevices();

        BEGIN_FUNCTION_MAP
            FUNCTION_0("getNetworkAdapterInformation", getNetworkAdapterInformation);
            FUNCTION_1("selectNetworkAdapter", selectNetworkAdapter);              
            FUNCTION_0("startSearchingForDevices", startSearchingForDevices);
        END_FUNCTION_MAP

    protected:      

        RaumserverInstaller::RaumserverInstaller raumserverInstallerObject;

        void onDeviceFoundForInstall(RaumserverInstaller::DeviceInformation);
        void onDeviceRemovedForInstall(RaumserverInstaller::DeviceInformation);
        void onDeviceInformationChanged(RaumserverInstaller::DeviceInformation);
        void onInstallProgressInformation(RaumserverInstaller::DeviceInstaller::DeviceInstallerProgressInfo);
        void onInstallCompleted(RaumserverInstaller::DeviceInstaller::DeviceInstallerProgressInfo);

        sigs::connections connections;

};


#endif