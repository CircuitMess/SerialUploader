#ifndef SERIALUPLOADER_UPLOADER_H
#define SERIALUPLOADER_UPLOADER_H


#include "Serial.h"

class Uploader {
public:
	Uploader(Serial* serial);

	bool setup();
	bool upload(char* dir);

private:
	Serial* serial;

};


#endif //SERIALUPLOADER_UPLOADER_H
