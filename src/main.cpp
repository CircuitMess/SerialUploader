#include <stdio.h>
#include <dirent.h>
#include "Serial.h"
#include "Uploader.h"

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("./uploader <directory> <port>\n");
		exit(1);
	}

	char* dir = argv[1];
	char* port = argv[2];

	DIR* d;
	if(!(d = opendir(dir))){
		printf("Directory doesn't exist or isn't a directory\n");
		exit(2);
	}else{
		closedir(d);
	}

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
