#ifndef GK_FILE_H
#define GK_FILE_H

#define FILE_SET SEEK_SET
#define FILE_CUR SEEK_CUR
#define FILE_END SEEK_END

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <utility>

class File
{
private:
	FILE* m_Handle;

public:
	File(const char* path, const char* mode);
	~File();

	bool IsOpen();
	void Close();

	FILE* GetHandle();

	void Seek(int origin, long int offset = 0L);
	void Read(void* buffer, size_t size, size_t count);
	long int Tell();

	int GetChar();

public:
	static FILE* Open(const char* path, const char* mode);
	static bool Read(const char* path, std::string& contents);
};

#endif // GK_FILE_H