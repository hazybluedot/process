#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Process.hpp"

Process::Process(const std::vector<char*>& args, bool verbose) : 
    verbose(verbose), 
    m_name(args[0]),
    m_pid((pid_t)NULL),
    m_writepipe {-1,-1},
    m_readpipe {-1,-1},
    m_pwrite((FILE*)NULL),
    m_pread((FILE*)NULL)
{
    if ((m_pid = fork()) < 0)
    {
	perror("Process fork");
	throw std::string("Process fork");
    } else if ( m_pid == 0 ) {
	/* child process */
	execvp(args[0], const_cast<char**>(&args[0]));
	perror("Process execvp");
	throw std::string("Process execvp");
    } else { 
	/* parent process */
	if (verbose)
	    std::cerr << "Process " << m_name << ": forked PID " << m_pid << std::endl;
    }
};

Process::~Process()
{
    if (verbose)
	std::cerr << "Process " << m_name << ": Entering ~Process()" << std::endl;
    
    kill(m_pid, SIGTERM);
    int status;
    pid_t pid = waitpid(m_pid, &status, 0);
    if (pid < 0)
	perror("~Process waitpid");

    if (verbose)
	std::cerr << "Process " << m_name << ": Leaving ~Process()" << std::endl;
};

