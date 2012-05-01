#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Process.hpp"
#include <algorithm>
#include <stdexcept>

void throw_runtime(std::string w, int err=0)
{
    std::string what(w);
    if (err != 0)
    {
	char buffer[100];
	sprintf(buffer, ": %s\n", strerror(err));
	what.append(buffer);
    } 
    throw std::runtime_error(what);
}

Process::Process(const std::vector<std::string>& argss, bool verbose) : 
    verbose(verbose), 
    m_name(argss[0]),
    m_pid((pid_t)NULL),
    m_writepipe {-1,-1},
    m_readpipe {-1,-1},
    m_pwrite((FILE*)NULL),
    m_pread((FILE*)NULL)
{
    if (argss.size() <= 0)
    {
	throw_runtime("Need at least 1 argument");
    }
    if (pipe(m_writepipe) < 0 || pipe(m_readpipe) < 0)
    {
	throw_runtime("Pipe:",errno);
    }

    if ((m_pid = fork()) < 0)
    {
	throw_runtime("Process fork", errno);
    } else if ( m_pid == 0 ) {
	/* child process */
	close(PARENT_READ);
	close(PARENT_WRITE);
	dup2(CHILD_WRITE,1); close(CHILD_WRITE);
	dup2(CHILD_READ,0); close(CHILD_READ);
	std::vector<const char*> args;
	std::transform(argss.begin(),argss.end(), std::back_inserter(args), [](std::string s)
		       {
			   return s.c_str();
		       } );
	args.push_back( NULL );
	execvp(args[0], const_cast<char**>(&args[0]));
	throw_runtime("Process execvp",errno);
    } else { 
	/* parent process */
	if (verbose)
	    std::cerr << "Process " << m_name << ": forked PID " << m_pid << std::endl;
	close(CHILD_READ);
	close(CHILD_WRITE);
	m_pread = fdopen(PARENT_READ, "r");
	m_pwrite = fdopen(PARENT_WRITE, "w");
    }
};

Process::~Process()
{
    if (verbose)
	std::cerr << "Process " << m_name << ": Entering ~Process()" << std::endl;
    
    fclose(m_pwrite);
    int status;
    pid_t pid = waitpid(m_pid, &status, 0);
    if (pid < 0)
	perror("~Process waitpid");
    fclose(m_pread);

    if (verbose)
	std::cerr << "Process " << m_name << ": Leaving ~Process()" << std::endl;
};

void Process::write(const std::string& line)
{
    fputs(line.c_str(), m_pwrite);
    fflush(m_pwrite);
}

std::string Process::read()
{
    char* mystring = NULL;
    size_t num_bytes;

    getline(&mystring, &num_bytes, m_pread);
    std::string line(mystring);
    return line;
}
