#ifndef GK_SHADER_SOURCE_HPP
#define GK_SHADER_SOURCE_HPP

class ShaderSource
{
private:
	bool m_is_literal;
	const char* m_data;

private:
	ShaderSource();

public:
	static ShaderSource FromFile(const char* path);
	static ShaderSource FromString(const char* str);
	ShaderSource(ShaderSource&& that); // move constructor

	~ShaderSource();

	bool IsValid() const;
	const char* GetData() const;
};

#endif // GK_SHADER_SOURCE_HPP
