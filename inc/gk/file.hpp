#ifndef GK_FILE_H
#define GK_FILE_H

#define FILE_SET SEEK_SET
#define FILE_CUR SEEK_CUR
#define FILE_END SEEK_END

#include <cstdio>
#include <cstdlib>

#include <string>
#include <utility>

class File
{
private:
	FILE* m_Handle;

public:
	enum
	{
		READ_BINARY = 0,
		READ_TEXT   = 1
	};

public:
	File(const char* path, unsigned int mode);
	~File();

	bool IsOpen();
	void Close();

	FILE* GetHandle();

	long int Tell();
	long int Size();

	bool Seek(int origin, long int offset = 0L);
	
	int ReadChar();

	template <typename T>
	bool Read(T* value, unsigned int count);
	bool Read(void* buffer, size_t size, size_t count);

public:
	static FILE* Open(const char* path, unsigned int mode);
	static bool Read(const char* path, std::string& contents);
};

#endif // GK_FILE_H