#include <gk/file.hpp>

#include <gk/assert.hpp>

File::File()
{
	m_Handle = NULL;
}

File* File::Open(const char* path, const char* mode)
{
	File* file = new File();
	file->m_Handle = fopen(path, mode);

	GK_ASSERT(file->m_Handle != nullptr, ("Error: File [%s] could not be opened for [%s]\n", path, mode));
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
	GK_ASSERT(fseek(m_Handle, offset, origin) == 0, ("File seek failure\n"));
}

void File::Read(void* buffer, size_t size, size_t count)
{
	GK_ASSERT(fread(buffer, size, count, m_Handle) == count, ("Error reading file\n"));
}

long int File::Tell()
{
	long int ret = ftell(m_Handle);
	GK_ASSERT(ret != -1L, ("File tell failure\n"));

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
