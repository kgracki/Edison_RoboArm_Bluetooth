extern "C" {
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
}

#include "Intel_Edison_BT_SPP.hpp"

using namespace std;

int Intel_Edison_BT_SPP::open() {

	printf("I am inside SPP open function\n");

	if (_fd != -1) {
		printf("fd is not equal to -1\n");
		close(_fd);
	}

	//open a FIFO file for reading only
	_fd = ::open(_pipeName, O_RDONLY);
	int fd_return = _fd;
	printf("_fd is equal to %d \n",fd_return);


	if (_fd == -1) {
		perror("Cannot open file\n");
		printf("errno %s\n", strerror(errno));

		return 0;
	}

	return -1;
}

/*
int Intel_Edison_BT_SPP::openIn() {

	if (_fd != -1) {
		printf("fd is not equal to -1\n");
		close(_fd);
	}


	_fd = ::open(_pipeNameIn, O_RDONLY);
	printf("I opened pipe file!\n");

	if (_fd == -1) {
		perror("Cannot open file\n");
		printf("errno %s\n", strerror(errno));

		return 0;
	}

	return -1;
}

ssize_t Intel_Edison_BT_SPP::write() {

	if (_fd == -1)
		openIn();

	ssize_t size = ::write(_fd, _buf, MAX_BUF-1);

	if (size == -1) {
		perror("Write error\n");
		printf("errno %s\n", strerror(errno));

	} else {
		_buf[size]=0;
//		perror("jeff test\n");
//		printf("ble_buf[%n] = \n",size );
	}

	return size;
}
*/

ssize_t Intel_Edison_BT_SPP::read() {

	if (_fd == -1)
		open();

	ssize_t size = ::read(_fd, _buf, MAX_BUF-1);

	if (size == -1) {
		perror("Read error\n");
		printf("errno %s\n", strerror(errno));
	} else {
		_buf[size]=0;
	}

	return size;
}

 char * Intel_Edison_BT_SPP::getBuf() {
	return _buf;
}

Intel_Edison_BT_SPP::~Intel_Edison_BT_SPP() {
	if (_fd != -1) {
		close (_fd);
		_fd = -1;
	}
}
