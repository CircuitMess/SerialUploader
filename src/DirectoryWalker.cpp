#include "DirectoryWalker.h"

#if defined(__CYGWIN__) || defined(_WIN64) || defined(_WIN32)
#define WINDOWS
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include <sstream>
#include <cstring>

#endif

DirectoryWalker::DirectoryWalker(onFileFunc onFile) : onFile(onFile){ }

bool DirectoryWalker::walk(const char* dir, const char* prefix){
#ifdef WINDOWS
	char dirSearch[PATH_MAX];
	sprintf(dirSearch, "%s\\*", dir);

	WIN32_FIND_DATA file;
	HANDLE hFind = FindFirstFile(dirSearch, &file);
	if(hFind == INVALID_HANDLE_VALUE){
		return false;
	}

	do {
		if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		char fullpath[PATH_MAX];
		sprintf(fullpath, "%s\\%s", dir, file.cFileName);

		onFile(file.cFileName, fullpath, file.nFileSizeLow);
	} while(FindNextFile(hFind, &file) != 0);

	FindClose(hFind);
#else
	DIR* dp = opendir(dir);
	if(dp == nullptr) return false;

	dirent* file;
	struct stat info;
	while((file = readdir(dp))){
		if(!(file->d_type & (DT_LNK | DT_REG | DT_DIR))) continue;

		char fullpath[PATH_MAX];
		sprintf(fullpath, "%s/%s", dir, file->d_name);

		if(file->d_type & DT_DIR){
			if(!strncmp(file->d_name, "..", 3) || !strncmp(file->d_name, ".", 2)) continue;

			std::stringstream str;
			str << prefix << file->d_name << "-";

			walk(fullpath, str.str().c_str());
			continue;
		}

		char filename[FILENAME_MAX];
		sprintf(filename, "%s%s", prefix, file->d_name);

		stat(fullpath, &info);
		onFile(filename, fullpath, info.st_size);
	}

	closedir(dp);
#endif

	return true;
}
