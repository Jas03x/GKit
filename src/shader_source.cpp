#include <gk/shader_source.hpp>

#include <gk/file.hpp>

ShaderSource::ShaderSource()
{
	m_data = nullptr;
	m_is_literal = false;
}

ShaderSource ShaderSource::FromFile(const char* path)
{
	ShaderSource source;

	std::string contents;
	if (File::Read(path, contents))
	{
		char* data = new char[contents.size()];
		memcpy(data, contents.data(), contents.size());

		source.m_data = data;
		source.m_is_literal = false;
	}

	return source;
}

ShaderSource ShaderSource::FromString(const char* str)
{
	ShaderSource source;
	source.m_data = str;
	source.m_is_literal = true;
	return source;
}

ShaderSource::ShaderSource(ShaderSource&& that)
{
	this->m_data = that.m_data;
	this->m_is_literal = that.m_is_literal;

	// invalidate old object
	that.m_data = nullptr;
	that.m_is_literal = false;
}

ShaderSource::~ShaderSource()
{
	if (!m_is_literal && (m_data != nullptr))
	{
		delete m_data;
	}

	m_data = nullptr;
	m_is_literal = false;
}

bool ShaderSource::IsValid() const
{
	return m_data != nullptr;
}

const char* ShaderSource::GetData() const
{
	return m_data;
}
