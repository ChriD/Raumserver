. Raumserver.

A app/service/deamon to control the Raumfeld Multiroomsystem via HTTP-Requests  
  
**Please check the [wiki](https://github.com/ChriD/Raumserver/wiki) for install information and how to use the server**  
  
# Changelog Raumserver

#### Version 1.0.3 (Under development)
* Add ability to save and load system states (eg. for doorbell ring) - (under development)

#### Version 1.0.2 (Under development)
* Add SIGSEV Handler to find crashing bugs - (under development)
* Add stresstest local webpage - (under development)
* Added JSON return data for RADIOTIME Media Items
* Inactive zones now return name in 'getRendererState' (new json key 'name')
* update UPNP Stack dependencies to the newest version - (under development)

#### Version 1.0.1 - 09.10.2016
* Added "Access-Control-Allow-Origin" header to the server json response
* Added  support for 3rd party web clients (fully operation webserver / use docroot folder)
* Added 3 new Request for handling the Standby state for the Gen.2 devices
* Fixing source parameter "upper lower case" problem on load shuffle request

#### Version 1.0.0 - Initial version  
* This is the initial version

  
# Changelog Raumserver Installer

#### Version 1.0.2 - (Under development)
* Updated instalSettings.xml for One M1 (use ARMv5 instead of ARMv7)
* Fixed the problem of permissions on starting script and execution files - (under development)
* Update sciter GUI dependencies to newest version - (under development)
  
#### Version 1.0.1 - 19.09.2016
* Added ability to set ssh user and passwort in the application settings file for the installer
* The file transfer mode of the installer was changed to scp (instead of sftp) because RF-devices with firmware higher than 1.56 do not support sftp anymore
 
#### Version 1.0.0 - Initial version  
* This is the initial version

