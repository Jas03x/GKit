#ifndef GK_FILE_H
#define GK_FILE_H

#define FILE_SET SEEK_SET
#define FILE_CUR SEEK_CUR
#define FILE_END SEEK_END

#include <stdio.h>
#include <stdlib.h>

#include <string>

class File
{
private:
	FILE* m_Handle;

	File();
public:
	static File* Open(const char* path, const char* mode);
	~File();

	FILE* GetHandle();

	void Seek(int origin, long int offset = 0L);
	void Read(void* buffer, size_t size, size_t count);
	long int Tell();

	static std::string Read(const char* path);
};

#endif // GK_FILE_H