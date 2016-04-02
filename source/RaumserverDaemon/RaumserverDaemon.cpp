#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <raumserver/raumserver.h>

#define DAEMON_NAME "raumsrvDaemon"

void process()
{

    syslog (LOG_NOTICE, "Writing to my Syslog");
}

int main(int argc, char *argv[])
{

    Raumserver::Raumserver  raumserverObject;

    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);


    syslog (LOG_NOTICE, "TEST");
    syslog(LOG_INFO, "Entering Daemon");

    pid_t pid, sid;

   //Fork the Parent Process
    pid = fork();

    if (pid < 0) {  syslog (LOG_NOTICE, "No PID"); exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) {  syslog (LOG_NOTICE, "PID OK"); exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) {  syslog (LOG_NOTICE, "NO SID"); exit(EXIT_FAILURE); }
     syslog (LOG_NOTICE, "GOT SID");

/*
     char szTmp[32];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
        */

    std::string fullFileName = "";

    // Code taken from: http://www.gamedev.net/community/forums/topic.asp?topic_id=459511
    std::string path = "";
    pid_t pid2 = getpid();
    char buf[20] = {0};
    sprintf(buf,"%d",pid2);
    std::string _link = "/proc/";
    _link.append( buf );
    _link.append( "/exe");
    char proc[512];
    int ch = readlink(_link.c_str(),proc,512);
    if (ch != -1)
    {
        proc[ch] = 0;
        path = proc;
        std::string::size_type t = path.find_last_of("/");
        path = path.substr(0,t);
    }
    fullFileName = path + std::string("/");


    syslog (LOG_NOTICE, fullFileName.c_str());

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
     syslog (LOG_NOTICE, "Close descripors");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //----------------
    //Main Process
    //----------------

    // TODO: init raumkernel
    // set log file to??

    // create raumserver object adn do init init
    raumserverObject.setSettingsFile(fullFileName + "raumserver.xml");
    raumserverObject.initLogObject(Raumkernel::Log::LogType::LOGTYPE_ERROR, fullFileName + "logs/");
    raumserverObject.init();
    /*
    raumserverObject = std::unique_ptr<Raumserver::Raumserver>(new Raumserver::Raumserver());
    raumserverObject->setSettingsFile(settingsFile);
    raumserverObject->initLogObject(Raumkernel::Log::LogType::LOGTYPE_ERROR, logFileDirectory);

    connections.connect(raumserverObject->getLogObject()->sigLog, this, &RaumserverService::onLog);

    raumserverObject->init();
    */


    while(true)
    {
        syslog (LOG_NOTICE, "Process");
        process();    //Run our Process
        sleep(60);    //Sleep for 60 seconds
    }

    syslog (LOG_NOTICE, "Daemon closed");

    //Close the log
    closelog ();

}

