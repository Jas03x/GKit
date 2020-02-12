#include <gk/file.hpp>

#include <assert.h>

File::File()
{
	m_Handle = NULL;
}

File* File::Open(const char* path, const char* mode)
{
	File* file = nullptr;
	FILE* handle = fopen(path, mode);

	if(handle != nullptr)
	{
		file = new File();
		file->m_Handle = handle;
	}
	else
	{
		printf("could not open file \"%s\" for mode \"%s\"\n", path, mode);
	}

	return file;
}

File::~File()
{
	if (m_Handle != nullptr)
	{
		fclose(m_Handle);
	}
}

FILE* File::GetHandle()
{
	return m_Handle;
}

void File::Seek(int origin, long int offset)
{
	assert(fseek(m_Handle, offset, origin) == 0);
}

void File::Read(void* buffer, size_t size, size_t count)
{
	assert(fread(buffer, size, count, m_Handle) == count);
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

std::pair<bool, std::string> File::Read(const char* path)
{
	std::pair<bool, std::string> ret;

	printf("reading: %s\n", path);

	File* file = File::Open(path, "rb");
	if (file == nullptr)
	{
		printf("error: could not open file [%s] for reading\n", path);
	}
	else
	{
		file->Seek(FILE_END);
		long int size = file->Tell();
		file->Seek(FILE_SET);

		ret.second.resize(size + 1, 0);
		
		char* str = &ret.second[0];
		for (unsigned int i = 0; i < size; i++)
		{
			int c = file->GetChar();
			if (c == EOF)
			{
				break;
			}
			else if (c != '\r')
			{
				*str = c;
				str++;
			}
		}

		ret.first = true;
		delete file;
	}
	
	return ret;
}
