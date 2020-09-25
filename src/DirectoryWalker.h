#ifndef SERIALUPLOADER_DIRECTORYWALKER_H
#define SERIALUPLOADER_DIRECTORYWALKER_H

#include <cstdint>
#include <functional>

typedef std::function<void(const char* name, const char* path, uint32_t size)> onFileFunc;

class DirectoryWalker {
public:
	DirectoryWalker(onFileFunc onFile);

	bool walk(const char* dir, const char* prefix = "");

private:
	onFileFunc onFile;

};


#endif //SERIALUPLOADER_DIRECTORYWALKER_H
