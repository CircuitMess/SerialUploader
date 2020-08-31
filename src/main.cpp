#include <stdio.h>
#include "Serial.h"
#include "Uploader.h"

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("./uploader <directory> <port>\n");
		exit(1);
	}

	char* dir = argv[1];
	char* port = argv[2];

	Serial serial(port);

	if(!serial.open()){
		printf("Error opening port %s\n", port);
		exit(2);
	}else{
		printf("Serial opened\n");
	}

	Uploader uploader(&serial);
	uploader.setup();
	uploader.upload(dir);

	serial.close();

	return 0;
}
