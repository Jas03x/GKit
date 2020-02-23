#include <gk/file.hpp>

#include <assert.h>

File::File(const char* path, const char* mode)
{
	m_Handle = File::Open(path, mode);
}

File::~File()
{
	this->Close();
}

FILE* File::Open(const char* path, const char* mode)
{
	FILE* handle = fopen(path, mode);

	if(handle == nullptr)
	{
		printf("could not open file \"%s\" for mode \"%s\"\n", path, mode);
	}

	return handle;
}

void File::Close()
{
	if (m_Handle != nullptr)
	{
		fclose(m_Handle);
		m_Handle = nullptr;
	}
}

bool File::IsOpen()
{
	return m_Handle != nullptr;
}

FILE* File::GetHandle()
{
	return m_Handle;
}

long int File::Size()
{
	long int cur = Tell();
	Seek(SEEK_END, 0L);
	long int size = Tell();
	Seek(SEEK_SET, cur);
	return size;
}

bool File::Seek(int origin, long int offset)
{
	return (fseek(m_Handle, offset, origin) == 0);
}

bool File::Read(void* buffer, size_t size, size_t count)
{
	return (fread(buffer, size, count, m_Handle) == count);
}

int File::GetChar()
{
	return fgetc(m_Handle);
}

long int File::Tell()
{
	long int ret = ftell(m_Handle);
	assert(ret != -1L);

	return ret;
}

bool File::Read(const char* path, std::string& contents)
{
	bool status = true;

	File file(path, "rb");
	if (file.IsOpen())
	{
		file.Seek(FILE_END);
		long int size = file.Tell() + 1;
		file.Seek(FILE_SET);

		contents.resize(size, 0);
		
		unsigned int len = 0;
		char* str = &contents[0];

		for (unsigned int i = 0; i < size; i++)
		{
			int c = file.GetChar();
			if (c == EOF)
			{
				break;
			}
			else if (c != '\r')
			{
				str[len++] = c;
			}
		}

		contents[len++] = 0; // null-terminate
		contents.resize(len);
	}
	
	return status;
}
