#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdint>
#include "Uploader.h"

#if defined(__CYGWIN__) || defined(_WIN64) || defined(_WIN32)
#define SEP "\\"
#else
#define SEP "/"
#endif

Uploader::Uploader(Serial* serial) : serial(serial){ }

bool Uploader::setup(){
	printf("Sending upload confirmation\n");
	serial->write((unsigned char*) "upload", 6);

	char inBuffer[5];
	serial->read(reinterpret_cast<unsigned char*>(inBuffer), 5);
	if(strncmp(inBuffer, "erase", 5)){
		printf("Expected 'erase', gotten '%.5s'\n", inBuffer);
		exit(3);
	}else{
		printf("Erasing...\n");
	}

	serial->read(reinterpret_cast<unsigned char*>(inBuffer), 5);
	if(strncmp(inBuffer, "ready", 5)){
		printf("Expected 'ready' , gotten '%.5s'\n", inBuffer);
		exit(3);
	}else{
		printf("Done, uploading now...\n");
	}

	return true;
}

uint32_t swendian(uint32_t value){
	uint32_t b0, b1, b2, b3;

	b0 = (value & 0x000000ff) << 24u;
	b1 = (value & 0x0000ff00) << 8u;
	b2 = (value & 0x00ff0000) >> 8u;
	b3 = (value & 0xff000000) >> 24u;

	return (b0 | b1 | b2 | b3);
}

bool Uploader::upload(char* dir){
	DIR* dp = opendir(dir);
	if(dp == nullptr) return false;

	dirent* file;
	struct stat info;
	char filename[MAX_FILENAME + 1];
	unsigned char data[256];
	while((file = readdir(dp))){
		if(!(file->d_type & (DT_LNK | DT_REG))) continue;

		sprintf(filename, "%.24s", file->d_name);
		filename[MAX_FILENAME] = 0;

		char fullpath[PATH_MAX];
		sprintf(fullpath, "%s%s%s", dir, SEP, file->d_name);
		stat(fullpath, &info);
		uint32_t filesize = info.st_size;

		printf("%s %u B\n", filename, filesize);

		serial->write(reinterpret_cast<unsigned char*>(filename), MAX_FILENAME + 1);
		serial->write(reinterpret_cast<unsigned char*>(&filesize), sizeof(uint32_t));

		FILE* file = fopen(fullpath, "r");
		size_t bytes;
		while((bytes = fread(data, 1, 256, file))){
			serial->write(data, bytes);
		}
		fclose(file);
	}

	closedir(dp);

	strncpy(filename, "end\0", 4);
	serial->write(reinterpret_cast<unsigned char*>(filename), 10);
}
