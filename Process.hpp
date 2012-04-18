#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <sys/types.h>
#include <sys/poll.h>
#include <iostream>
#include <vector>

#define	PARENT_READ     m_readpipe[0]
#define	CHILD_WRITE	m_readpipe[1]
#define CHILD_READ      m_writepipe[0]
#define PARENT_WRITE	m_writepipe[1]

class Process
{
public:
    //The extra bool argument can be used to enable verbose messages
    Process(const std::vector<std::string>&, bool verbose=false);
    
    // prevent copying and assignment
    Process(const Process &p);
    Process& operator=(const Process &p);

    //Implement a move constructor   
    Process(Process&& other);
    Process& operator=(Process&& other);

    ~Process();

    void write(const std::string&);
    std::string read();

    int wait();
    bool stop();
    bool resume();
    int status();

private:
    bool verbose;
    std::string m_name;

    //PID of child process
    pid_t m_pid;
    int m_status;

    //File descriptors for writing/reading to/from process
    int m_writepipe[2];
    int m_readpipe[2];

    //File stream to write/read to/from child process
    FILE* m_pwrite;
    FILE* m_pread;
};

#endif
