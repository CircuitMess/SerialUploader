#include "Serial.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

Serial::Serial(const char* port){
	this->port = port;
}

bool Serial::open(){
	fd = ::open(port, O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC);
	if(fd == -1) return false;

	fcntl(fd, F_SETFL, 0);

	termios options;
	tcgetattr(fd, &options);

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |=  CS8;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CREAD | CLOCAL;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_iflag &= ~(ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;
	options.c_cc[VMIN] = 1;
	options.c_cc[VTIME] = 0;

	tcflush(fd, TCOFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	return true;
}

size_t Serial::write(unsigned char* buffer, size_t size){
	if(fd == -1) return -1;
	return ::write(fd, buffer, size);
}

size_t Serial::read(unsigned char* buffer, size_t size){
	if(fd == -1) return -1;
	return ::read(fd, buffer, size);
}

void Serial::close(){
	if(fd == -1) return;
	::close(fd);
}
