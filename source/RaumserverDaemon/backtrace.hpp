#ifndef BACKTRACE_H
#define BACKTRACE_H


namespace Backtrace
{


#ifndef _MSC_VER

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <iostream>
#include <cxxabi.h>
#include <ctime>

	

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


#endif


#ifdef _MSC_VER

	inline void AddSignalHandlers()
	{
		// Dummy for windows
	}

#endif 

} // namespace SignalFault

#endif

