#include <gk/core/io/file.hpp>

#include <cassert>

File::File(const char* path, unsigned int mode)
{
	m_Handle = File::Open(path, mode);
}

File::~File()
{
	this->Close();
}

FILE* File::Open(const char* path, unsigned int mode)
{
	static const char* MODE_TABLE[] =
	{
		"rb", // READ_BINARY (default)
		"r"   // READ_TEXT
	};
	static const unsigned int MODE_COUNT = sizeof(MODE_TABLE) / sizeof(const char*);

	const char* file_mode = (mode < MODE_COUNT) ? MODE_TABLE[mode] : MODE_TABLE[0];

	FILE* handle = fopen(path, file_mode);
	if(handle == nullptr)
	{
		printf("could not open file \"%s\" for mode \"%s\"\n", path, file_mode);
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

long int File::Tell()
{
	long int ret = ftell(m_Handle);
	if (ret == -1L) {
		printf("ftell failure\n");
	}
	return ret;
}

bool File::Seek(int origin, long int offset)
{
	bool result = (fseek(m_Handle, offset, origin) == 0);
	if (!result) {
		printf("fseek failure\n");
	}
	return result;
}

int File::ReadChar()
{
	return fgetc(m_Handle);
}

template <typename T>
bool File::Read(T* value, unsigned int count)
{
	return this->Read(value, sizeof(T), count);
}

// instantiate templates for supported data types
template bool File::Read(char* value, unsigned int count);

template bool File::Read(int8_t*   value, unsigned int count);
template bool File::Read(uint8_t*  value, unsigned int count);
template bool File::Read(int16_t*  value, unsigned int count);
template bool File::Read(uint16_t* value, unsigned int count);
template bool File::Read(int32_t*  value, unsigned int count);
template bool File::Read(uint32_t* value, unsigned int count);
template bool File::Read(int64_t*  value, unsigned int count);
template bool File::Read(uint64_t* value, unsigned int count);
template bool File::Read(float*    value, unsigned int count);
template bool File::Read(double*   value, unsigned int count);

bool File::Read(void* buffer, size_t size, size_t count)
{
	bool result = (fread(buffer, size, count, m_Handle) == count);
	if (!result) {
		printf("fread failure\n");
	}
	return result;
}

bool File::Read(const char* path, std::string& contents)
{
	bool status = true;

	File file(path, File::READ_BINARY);
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
			int c = file.ReadChar();
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
