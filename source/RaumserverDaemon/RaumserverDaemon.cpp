#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <execinfo.h>
#include <signal.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <iostream>
#include <cxxabi.h>
#include <ctime>


#ifdef __arm__
  #include <libunwind.h>
  #include <libunwind-arm.h>
#else
  #include <libunwind.h>
  #include <libunwind-x86_64.h>
#endif



//#include "client/linux/handler/exception_handler.h"

#include <cxxabi.h>

#include <raumserver/raumserver.h>


#define DAEMON_NAME "raumsrvDaemon"

    std::shared_ptr<Raumkernel::Log::Log> gLog;

	/* This structure mirrors the one found in /usr/include/asm/ucontext.h */
	typedef struct _sig_ucontext {
		unsigned long     uc_flags;
		struct ucontext   *uc_link;
		stack_t           uc_stack;
		struct sigcontext uc_mcontext;
		sigset_t          uc_sigmask;
	} sig_ucontext_t;


void backtrace(void * reserved)
{

  unw_cursor_t cursor;
  unw_context_t context;

  unw_getcontext(&context);

  unw_init_local(&cursor, &context);

  
  std::string bigbuf;

  int n=0;
  int err= unw_step(&cursor);
  while ( err ) {
    unw_word_t ip, sp, off;

    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);

    char symbol[256] = {"<unknown>"};
    char buffer[256];
    char *name = symbol;

    if ( !unw_get_proc_name(&cursor, symbol, sizeof(symbol), &off) ) {
      int status;
      if ( (name = abi::__cxa_demangle(symbol, NULL, NULL, &status)) == 0 )
        name = symbol;
    }

    sprintf(buffer, "#%-2d 0x%016" PRIxPTR " sp=0x%016" PRIxPTR " %s + 0x%" PRIxPTR "\n",
        ++n,
        static_cast<uintptr_t>(ip),
        static_cast<uintptr_t>(sp),
        name,
        static_cast<uintptr_t>(off));

    std::string sbuf(buffer);
    bigbuf += sbuf;
   

    if ( name != symbol )
      free(name);

    err= unw_step(&cursor);
  }

  bigbuf = "BAD SIGERROR! Step Return: " + std::to_string(err) + "\n" + bigbuf;
  gLog->critical(bigbuf, CURRENT_POSITION);


}



void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
{
	
	backtrace(ucontext);	
        gLog->critical("SIGNAL FAULT! Program can not continue!", CURRENT_POSITION);
	exit(EXIT_FAILURE);
}


inline void AddSignalHandlers()
{	
	struct sigaction sigact;
	sigact.sa_sigaction = crit_err_hdlr;
	sigact.sa_flags = SA_RESTART | SA_SIGINFO;
	if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0)
	{
		fprintf(stderr, "error setting signal handler for %d (%s)\n",
			SIGSEGV, strsignal(SIGSEGV));
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGABRT, &sigact, (struct sigaction *)NULL) != 0)
	{
		fprintf(stderr, "error setting signal handler for %d (%s)\n",
			SIGABRT, strsignal(SIGABRT));
		exit(EXIT_FAILURE);
	}

}


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

/*
static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
void* context, bool succeeded) {
  //printf("Dump path: %s\n", );
  std::string s(descriptor.path());
  gLog->critical("Dump stored: " + s, CURRENT_POSITION);
  gLog->critical("SIGNAL FAULT! Program can not continue!", CURRENT_POSITION);
  return succeeded;
}*/


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
    //raumserverObject.addSystemSignalHandlers();
    AddSignalHandlers();

    //google_breakpad::MinidumpDescriptor descriptor(workingDirectory + "logs/fault");
    //google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);

    // set the log adapters we want to use (because we do not want to use the standard ones which includes console output)
    std::vector<std::shared_ptr<Raumkernel::Log::LogAdapter>> adapters;
    auto logAdapterFile = std::shared_ptr<Raumkernel::Log::LogAdapter_File>(new Raumkernel::Log::LogAdapter_File());
    logAdapterFile->setLogFilePath(workingDirectory + "logs/");
    adapters.push_back(logAdapterFile);

    // create raumserver object and do init init
    raumserverObject.setSettingsFile(workingDirectory + "settings.xml");
    raumserverObject.initLogObject(Raumkernel::Log::LogType::LOGTYPE_ERROR, workingDirectory + "logs/", adapters);
    raumserverObject.init();
    
    gLog = raumserverObject.getLogObject();
    gLog->debug("Deamon test log", CURRENT_POSITION);

    // go into an endless loop and wait until daemon is killed by the syste,
    while(true)
    {
        sleep(60);    //Sleep for 60 seconds
    }

    syslog (LOG_NOTICE, "Daemon closed");

    //Close the log
    closelog ();

}

