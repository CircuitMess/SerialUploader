#include <cstring>
#include "Uploader.h"
#include "DirectoryWalker.h"

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
	char filename[MAX_FILENAME + 1];
	unsigned char data[256];

	DirectoryWalker walker([&](const char* name, const char* path, uint32_t size){
		printf("%s %u B\n", name, size);

		sprintf(filename, "%.254s", name);
		filename[MAX_FILENAME] = 0;

		FILE* file = fopen(path, "rb");
		uint32_t sentSum;
		uint32_t sum;
		uint8_t retries = 0;
		do {
			serial->write(reinterpret_cast<unsigned char*>(filename), MAX_FILENAME + 1);
			serial->write(reinterpret_cast<unsigned char*>(&size), sizeof(uint32_t));

			fseek(file, 0, SEEK_SET);
			sentSum = 0;
			sum = 0;

			size_t bytes;
			while((bytes = fread(data, 1, 256, file))){
				serial->write(data, bytes);

				for(int i = 0; i < bytes; i += sizeof(uint32_t)){
					sum += *reinterpret_cast<uint32_t*>(&data);
				}
			}

			serial->read(reinterpret_cast<unsigned char*>(&sentSum), sizeof(uint32_t));
			if(sentSum != sum){
				printf("Checksum mismatch. Device received %u, expected %u\n", sentSum, sum);
				if (++retries > 3) {
					printf("Max retries reached...\n");
					break;
				}
			}
		}while(sentSum != sum);

		fclose(file);
	});

	bool success;
	if((success = walker.walk(dir))){
		strncpy(filename, "end\0", 4);
		serial->write(reinterpret_cast<unsigned char*>(filename), 10);
	}

	return success;
}
