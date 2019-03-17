#ifndef GK_SHADER_H
#define GK_SHADER_H

#include <gk/config.hpp>
#include <gk/graphics.hpp>

class Shader
{
private:
	GFX_HANDLE m_Handle;

protected:
	int GetAttributeLocation(const char* name);
	int GetUniformLocation(const char* name);

	void Load(const char* vsrc, const char* fsrc, void(*pfn_bind)(GFX_HANDLE));

public:
	Shader();
	Shader(const char* vsrc, const char* fsrc, void (*pfn_bind)(GFX_HANDLE));
	~Shader();

	void Bind() const;
};

#endif // GK_SHADER_H
