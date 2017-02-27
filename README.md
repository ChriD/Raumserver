
## ATTENTION!! ##
The C++ raumserver will be discontinued!  
Instead there will be a NodeJs raumserver!  
That means everywhere where  nodeJs may run you can run the raumserver and in fact NodeJs can be run on the armv7 raumfeld devices. So please have a look at the new repo which is also available on npm and stay tuned!
https://github.com/ChriD/node-raumserver
  
  
  
## Raumserver ##

A app/service/deamon to control the Raumfeld Multiroomsystem via HTTP-Requests  
  
**Please check the [wiki](https://github.com/ChriD/Raumserver/wiki) for install information and how to use the server**  
  
# Changelog Raumserver

#### Version 1.0.3 (Under development)
* Add ability to save and load system states (eg. for doorbell ring) - (under development)
* All returns should be in json formate! Even exceptions aso... - (under development)
* Add SIGSEV Handler for ARM devices - (under development)
* Add stresstest local webpage - (under development)

#### Version 1.0.2 - 11.02.2017
* Added SIGSEV handler for X64 bases machines
* Added JSON return data for RADIOTIME Media Items
* Inactive zones now return name in 'getRendererState' (new json key 'name')
* update UPNP Stack dependencies to the newest version
* added "updateId" and "sessionId" to expose and allow headers
* responses to requests are now always in json format
* some major changes in the kernel (switching from shared pointers to std pointers for device objects) and therfore rearrangeing the lock/unlock logic
* fixed LongProlling problem for renderer state request
* fixed problem that transport state did not change in the rendere state request

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
* Fixed the problem of permissions on starting script and execution files
* Update sciter GUI dependencies to newest version
  
#### Version 1.0.1 - 19.09.2016
* Added ability to set ssh user and passwort in the application settings file for the installer
* The file transfer mode of the installer was changed to scp (instead of sftp) because RF-devices with firmware higher than 1.56 do not support sftp anymore
 
#### Version 1.0.0 - Initial version  
* This is the initial version

