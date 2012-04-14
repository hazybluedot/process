#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

void signal_handler(int signo)
{
    cout <<  endl;
    signal(signo, SIG_DFL);
    raise(signo);
}

int main()
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    size_t nn=0;
    while(1)
    {
	cout << nn++ << " " << flush;
	sleep(1);
    }
    
    return EXIT_SUCCESS;
}
