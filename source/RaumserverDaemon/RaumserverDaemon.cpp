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

	void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
	{
		void *				caller_address;
		sig_ucontext_t *	uc;
		std::tm*			timeinfo;
		char				buffer [80];
		// cerr out to file!
		time_t t = time(0);   
		std::string file = "fault/fault_";

		timeinfo = std::localtime(&t);
		std::strftime(buffer,80,"%Y%m%d_%H%M%S", timeinfo);

		file += buffer;
		file += ".log";
		std::ofstream trace_log(file);
		std::streambuf *cerrbuf = std::cerr.rdbuf();
		std::cerr.rdbuf(trace_log.rdbuf());

		uc = (sig_ucontext_t *)ucontext;

		/* Get the address at the time the signal was raised */
		#if defined(__i386__) // gcc specific
				caller_address = (void *)uc->uc_mcontext.eip; // EIP: x86 specific
		#elif defined(__x86_64__) // gcc specific
				caller_address = (void *)uc->uc_mcontext.rip; // RIP: x86_64 specific
		#elif defined(__arm__) // gcc specific
				caller_address = (void *)uc->uc_mcontext.arm_pc;
		#else
		#error Unsupported architecture. 
		#endif

		std::cerr << "signal " << sig_num
			<< " (" << strsignal(sig_num) << "), address is "
			<< info->si_addr << " from " << caller_address
			<< std::endl << std::endl;
		void * array[50];
		int size = backtrace(array, 50);
		array[1] = caller_address;
		char ** messages = backtrace_symbols(array, size);
		// skip first stack frame (points here)
		for (int i = 1; i < size && messages != NULL; ++i)
		{
			char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;
			// find parantheses and +address offset surrounding mangled name
			for (char *p = messages[i]; *p; ++p)
			{
				if (*p == '(')
				{
					mangled_name = p;
				}
				else if (*p == '+')
				{
					offset_begin = p;
				}
				else if (*p == ')')
				{
					offset_end = p;
					break;
				}
			}
			// if the line could be processed, attempt to demangle the symbol
			if (mangled_name && offset_begin && offset_end &&
				mangled_name < offset_begin)
			{
				*mangled_name++ = '\0';
				*offset_begin++ = '\0';
				*offset_end++ = '\0';
				int status;
				char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);
				// if demangling is successful, output the demangled function name
				if (status == 0)
				{
					std::cerr << "[bt]: (" << i << ") " << messages[i] << " : "
						<< real_name << "+" << offset_begin << offset_end
						<< std::endl;
				}
				// otherwise, output the mangled function name
				else
				{
					std::cerr << "[bt]: (" << i << ") " << messages[i] << " : "
						<< mangled_name << "+" << offset_begin << offset_end
						<< std::endl;
				}
				free(real_name);
			}
			// otherwise, print the whole line
			else
			{
				std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
			}
		}
		std::cerr << std::endl;
		free(messages);

		std::cerr.flush();
		std::cerr.rdbuf(cerrbuf);

		std::cout << "SIGNAL FAULT! Check files in fault directory";
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

    // set the log adapters we want to use (because we do not want to use the standard ones which includes console output)
    std::vector<std::shared_ptr<Raumkernel::Log::LogAdapter>> adapters;
    auto logAdapterFile = std::shared_ptr<Raumkernel::Log::LogAdapter_File>(new Raumkernel::Log::LogAdapter_File());
    logAdapterFile->setLogFilePath(workingDirectory + "logs/");
    adapters.push_back(logAdapterFile);

    // create raumserver object and do init init
    raumserverObject.setSettingsFile(workingDirectory + "settings.xml");
    raumserverObject.initLogObject(Raumkernel::Log::LogType::LOGTYPE_ERROR, workingDirectory + "logs/", adapters);
    raumserverObject.init();
    
    gLog = raumserverObject->getLogObject();
    gLog->logDebug("Deamon test log", CURRENT_POSITION);

    // go into an endless loop and wait until daemon is killed by the syste,
    while(true)
    {
        sleep(60);    //Sleep for 60 seconds
    }

    syslog (LOG_NOTICE, "Daemon closed");

    //Close the log
    closelog ();

}

