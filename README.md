# SerialUploader
Uploads files via serial port. Originally developed for file upload to the [Spencer](https://www.circuitmess.com/spencer/) flash chip.

Device should be running a corresponding upload program. An example is the ```FlashTools::upload()``` function located in src/Util/FlashTools.cpp of the [Spencer repository](https://github.com/CircuitMess/Spencer).

## Usage
```shell script
./SerialUploader <directory> <port>
```

Where ```<direcory>``` is the directory containing files to be uploaded, and ```<port>``` is the device serial port. Only files in the directory will be uploaded, sub-directories will be ignored.

## Building
CMake, GNU Make, and gcc (g++) are required for compiling. On Windows, MinGW64-w64 can be used. Run ```cmake .``` in the root directory to generate build scripts, followed by ```make``` to build the program.

On Windows, run CMake as ```cmake . -G "MinGW Makefiles"``` to generate build scripts using the MinGW compiler.

## Protocol
The device is waiting for a 6 character string `upload`, after which it sends a 5 character string `erase`, beginning the flash chip erase operation, after which it send a 5 character string `ready`.

After receiving the ready string, the uploader starts sending files. For each file, first the filename is sent as a 25 character ASCII encoded string, where the last (25th, count starting at 1) character is the null-terminator. The following 4 bytes sent are the file size as an unsigned 32 bit integer.

Following the file metadata, the whole file is read from disk and sent as a raw byte stream, in chunks of 256 bytes.

After each file, the uploader expects to receive a 32 bit unsigned integer file checksum, calculated by summing each byte of the file as an unsigned integer. If the received checksum doesn't match the file, that file is re-sent. 

After all files in the directory are uploaded, a 4 character string `end\0` is sent, indicating the end of the upload operation.
