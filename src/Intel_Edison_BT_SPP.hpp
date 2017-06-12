#ifndef _INTEL_EDISON_BT_SPP_HPP_
#define _INTEL_EDISON_BT_SPP_HPP_

#if 0 // No need for IoT Dev Kit
 #include "WProgram.h"
#endif

#define MAX_BUF 4096

using namespace std;

class Intel_Edison_BT_SPP {
	public:
	int open();
	//int openIn();
	int read();
	//int write();
	 char * getBuf();
	~Intel_Edison_BT_SPP();

	private:
	//file descriptor
	int _fd = -1;
	//destination of the fifo file
	const char * _pipeName = "/tmp/ble_pipe_out";
	//const char * _pipeNameIn = "/tmp/ble_pipe_in";
	char _buf[MAX_BUF];
};

#endif
