#if defined(__CYGWIN__) || defined(_WIN64) || defined(_WIN32)
#define WINDOWS
#include "Serial.windows.impl"
#else
#include "Serial.posix.impl"
#endif