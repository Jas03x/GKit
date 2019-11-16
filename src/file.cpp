#include <gk/file.hpp>

#include <assert.h>

File::File()
{
	m_Handle = NULL;
}

File* File::Open(const char* path, const char* mode)
{
	File* file = new File();
	file->m_Handle = fopen(path, mode);

	assert(file->m_Handle != nullptr);
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

long int File::Tell()
{
	long int ret = ftell(m_Handle);
	assert(ret != -1L);

	return ret;
}

std::string File::Read(const char* name)
{
	File* file = File::Open(name, "rb");
	
	file->Seek(FILE_END);	
	long int size = file->Tell();
	file->Seek(FILE_SET);

	std::string data(size, 0);
	file->Read(&data[0], 1, size);
	
	delete file;
	return data;
}
