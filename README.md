# Raumserver

A app/service/deamon to control the Raumfeld Multiroomsystem via HTTP-Requests  
  
**Please check the [wiki](https://github.com/ChriD/Raumserver/wiki) for install information and how to use the server**  
  
# Changelog Raumserver

#### Version 1.0.2 (Under development)
* Add ability to save and load system states (eg. for doorbell ring) - (under development)

#### Version 1.0.1 (Under development)
* Added "Access-Control-Allow-Origin" header to the server json response
* Added  support for 3rd party web clients (fully operation webserver)
* Fixing source parameter "upper lower case" problem on load shuffle request

#### Version 1.0.0 - Initial version  
* This is the initial version

  
# Changelog Raumserver Installer
  
#### Version 1.0.1 - 19.09.2016
* Added ability to set ssh user and passwort in the application settings file for the installer
* The file transfer mode of the installer was changed to scp (instead of sftp) because RF-devices with firmware higher than 1.56 do not support sftp anymore
 
#### Version 1.0.0 - Initial version  
* This is the initial version

