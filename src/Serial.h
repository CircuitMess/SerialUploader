#ifndef SERIALUPLOADER_SERIAL_H
#define SERIALUPLOADER_SERIAL_H


#include <cstdlib>

class Serial {
public:
	Serial(const char* port);

	bool open();
	void close();

	size_t write(unsigned char* buffer, size_t size);
	size_t read(unsigned char* buffer, size_t size);

private:
	const char* port;
	int fd = -1;

};


#endif //SERIALUPLOADER_SERIAL_H
