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


std::string getWorkingDirectory()
{
    std::string path = "";
	syslog (LOG_NOTICE, "Getting PID");
    pid_t pid2 = getpid();
    char buf[20] = {0};
	syslog (LOG_NOTICE, "1");
    sprintf(buf,"%d",pid2);
    std::string _link = "/proc/";
	syslog (LOG_NOTICE, "2");
    _link.append( buf );
    _link.append( "/exe");
	syslog (LOG_NOTICE, "3");
    char proc[512];
	syslog (LOG_NOTICE, "readlink");
    int ch = readlink(_link.c_str(),proc,512);
	syslog (LOG_NOTICE, "4");
    if (ch != -1)
    {
        proc[ch] = 0;
        path = proc;
		syslog (LOG_NOTICE, "5");
        std::string::size_type t = path.find_last_of("/");
        path = path.substr(0,t);
    }
	syslog (LOG_NOTICE, "6");
    return path + std::string("/");
}


int main(int argc, char *argv[])
{
    Raumserver::Raumserver  raumserverObject;

    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_NOTICE, "Trying to start daemon");

    pid_t pid, sid;

   //Fork the Parent Process
    pid = fork();

    if (pid < 0) {  syslog (LOG_NOTICE, "Error forking the parent process"); exit(EXIT_FAILURE); }
    //We got a good pid, Close the Parent Process
    if (pid > 0) {  syslog (LOG_NOTICE, "Got pid, closing parent process"); exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) {  syslog (LOG_NOTICE, "Signature ID for child process could not be created!"); exit(EXIT_FAILURE); }
    syslog (LOG_NOTICE, "Daemon ready, initialize raumserver object");

    // get the working directory of the executable
    std::string workingDirectory = getWorkingDirectory();

    syslog (LOG_NOTICE, std::string("Working directory: " + workingDirectory).c_str());

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // Add some system signal handlers for crash reporting
    raumserverObject.addSystemSignalHandlers();

    // set the log adapters we want to use (because we do not want to use the standard ones which includes console output)
    std::vector<std::shared_ptr<Raumkernel::Log::LogAdapter>> adapters;
    auto logAdapterFile = std::shared_ptr<Raumkernel::Log::LogAdapter_File>(new Raumkernel::Log::LogAdapter_File());
    logAdapterFile->setLogFilePath(workingDirectory + "logs/");
    adapters.push_back(logAdapterFile);

    // create raumserver object and do init init
    raumserverObject.setSettingsFile(workingDirectory + "settings.xml");
    raumserverObject.initLogObject(Raumkernel::Log::LogType::LOGTYPE_ERROR, workingDirectory + "logs/", adapters);
    raumserverObject.init();

    // go into an endless loop and wait until daemon is killed by the syste,
    while(true)
    {
        sleep(60);    //Sleep for 60 seconds
    }

    syslog (LOG_NOTICE, "Daemon closed");

    //Close the log
    closelog ();

}

