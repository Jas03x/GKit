#ifndef GK_SHADER_H
#define GK_SHADER_H

#include <gk/graphics.hpp>
#include <gk/shader_source.hpp>

class Shader
{
private:
	GFX_HANDLE m_Handle;

protected:
	bool Load(const ShaderSource& vsrc, const ShaderSource& fsrc, void(*pfn_bind)(GFX_HANDLE));

public:
	Shader();
	~Shader();

	void Bind() const;
	int GetAttributeLocation(const char* name);
	int GetUniformLocation(const char* name);
};

#endif // GK_SHADER_H
