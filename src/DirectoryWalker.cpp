#include "DirectoryWalker.h"

#if defined(__CYGWIN__) || defined(_WIN64) || defined(_WIN32)
#define WINDOWS
#define SEP "\\"

#include <windows.h>
#else
#define SEP "/"

#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>

#endif

DirectoryWalker::DirectoryWalker(onFileFunc onFile) : onFile(onFile){ }

bool DirectoryWalker::walk(const char* dir){
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
		onFile(file.cFileName, file.nFileSizeLow);
	} while(FindNextFile(hFind, &file) != 0);

	FindClose(hFind);
#else
	DIR* dp = opendir(dir);
	if(dp == nullptr) return false;

	dirent* file;
	struct stat info;
	while((file = readdir(dp))){
		if(!(file->d_type & (DT_LNK | DT_REG))) continue;

		char fullpath[PATH_MAX];
		sprintf(fullpath, "%s%s%s", dir, SEP, file->d_name);
		stat(fullpath, &info);

		onFile(file->d_name, fullpath, info.st_size);
	}

	closedir(dp);
#endif

	return true;
}
