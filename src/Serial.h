#ifndef SERIALUPLOADER_SERIAL_H
#define SERIALUPLOADER_SERIAL_H

#include <cstdlib>

#ifdef WINDOWS
#include <windows.h>
#endif

class Serial {
public:
	Serial(const char* port);

	bool open();
	void close();

	size_t write(unsigned char* buffer, size_t size);
	size_t read(unsigned char* buffer, size_t size);

private:
	const char* port;

#ifdef WINDOWS
	HANDLE fd;
#else
	int fd = -1;
#endif

};


#endif //SERIALUPLOADER_SERIAL_H
