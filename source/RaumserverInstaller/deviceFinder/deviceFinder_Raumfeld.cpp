
#include "deviceFinder/deviceFinder_Raumfeld.h"


namespace RaumserverInstaller
{
    namespace DeviceFinder
    {

        DeviceFinder_Raumfeld::DeviceFinder_Raumfeld() : DeviceFinder()
        {
            selectedAdapterId = 0;
        }


        DeviceFinder_Raumfeld::~DeviceFinder_Raumfeld()
        {
            logDebug("Closing OpenHome UPNP Control Stack", CURRENT_POSITION);
            OpenHome::Net::UpnpLibrary::Close();
        }


        void DeviceFinder_Raumfeld::init()            
        {
            DeviceFinder::init();
            
            OpenHome::Net::InitialisationParams*		initParams;
            std::vector<OpenHome::NetworkAdapter*>*		networkAdapterList;
            std::uint8_t                                networkAdapterIdx = -1;
            TIpAddress									networkAdapterAddress;

            try
            {
                          
                logDebug("Init OpenHome UPNP Control Stack", CURRENT_POSITION);

                initParams = OpenHome::Net::InitialisationParams::Create();
                OpenHome::Net::UpnpLibrary::Initialise(initParams);    

                // after initialisation get the available network adapters
                networkAdapterInfomationList = getNetworkAdaptersInformation();

            }
            catch (Raumkernel::Exception::RaumkernelException &e)
            {
                if (e.type() == Raumkernel::Exception::ExceptionType::EXCEPTIONTYPE_APPCRASH)
                    throw e;
            }
            catch (std::exception &e)
            {
                throw e;
            }
            catch (std::string &e)
            {
                throw e;
            }
            catch (OpenHome::Exception &e)
            {
                throw e;
            }
            catch (...)
            {
                throw std::runtime_error("Unknown exception! [DeviceFinder_Raumfeld::init]");
            }
        }


        void DeviceFinder_Raumfeld::selectAdapterId(const std::uint8_t &_adapterId)
        {
            selectedAdapterId = _adapterId;
        }


        void DeviceFinder_Raumfeld::startSearchingForDevices()
        {
            if (!networkAdapterInfomationList.size())
                return;

            if (selectedAdapterId <= 0)
                return;

            if (selectedAdapterId >= networkAdapterInfomationList.size())
                return;

            NetworkAdaperInformation selectedAdapterInformation =  networkAdapterInfomationList[selectedAdapterId - 1];

            logInfo("Starting OpenHome UPNP Control Stack with network adapter: " + selectedAdapterInformation.name, CURRENT_POSITION);
            OpenHome::Net::UpnpLibrary::StartCp(selectedAdapterInformation.address);


            // TODO: @@@
        }


        void DeviceFinder_Raumfeld::loadNetworkAdaptersInformation()
        {
            std::vector<OpenHome::NetworkAdapter*>		*networkAdapterList;                   

            try
            {

                networkAdapterInfomationList.clear();

                logDebug("Retrieve network adapter list", CURRENT_POSITION);
                networkAdapterList = OpenHome::Net::UpnpLibrary::CreateSubnetList();

                if ((*networkAdapterList).size() == 0)
                {
                    logCritical("No network adapter available!", CURRENT_POSITION);
                    throw Raumkernel::Exception::RaumkernelException(Raumkernel::Exception::ExceptionType::EXCEPTIONTYPE_APPCRASH, CURRENT_POSITION, "Unrecoverable error! ABORT!", 200);
                }

                // iterate through the list and try to find the adapter with the given name
                for (std::uint8_t i = 0; i < (*networkAdapterList).size(); i++)
                {
                    std::string adapterName = (*networkAdapterList)[i]->Name();
                    logDebug("Adapter " + std::to_string(i) + ": " + adapterName, CURRENT_POSITION);

                    NetworkAdaperInformation adapterInfo;
                    adapterInfo.id = i;
                    adapterInfo.name = (*networkAdapterList)[i]->Name();
                    adapterInfo.fullName = (*networkAdapterList)[i]->FullName();
                    adapterInfo.address = (*networkAdapterList)[i]->Subnet();
                    networkAdapterInfomationList.push_back(adapterInfo);
                }

                // clean up the subnet list, we do not need it anymore...
                OpenHome::Net::UpnpLibrary::DestroySubnetList(networkAdapterList);

            }
            catch (Raumkernel::Exception::RaumkernelException &e)
            {
                if (e.type() == Raumkernel::Exception::ExceptionType::EXCEPTIONTYPE_APPCRASH)
                    throw e;
            }
            catch (std::exception &e)
            {
                throw e;
            }
            catch (std::string &e)
            {
                throw e;
            }
            catch (OpenHome::Exception &e)
            {
                throw e;
            }
            catch (...)
            {
                throw std::runtime_error("Unknown exception! [DeviceFinder_Raumfeld::getNetworkAdaptersInformation]");
            }            
        }


        std::vector<NetworkAdaperInformation> DeviceFinder_Raumfeld::getNetworkAdaptersInformation()
        {
            return networkAdapterInfomationList;
        }


    }
}
