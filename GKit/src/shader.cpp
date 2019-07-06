#include <gk/shader.hpp>

#include <assert.h>
#include <stdio.h>

#include <string>

bool _check_status (
	GFX_HANDLE handle,
	uint pname,
	const char* operation,
	void (*getObjIv)(uint32_t, uint32_t, int32_t*),
	void (*getObjInfoLog)(uint32_t, int32_t, int32_t*, char*))
{
	int status = 0;
	getObjIv(handle, pname, &status);
	if (status != GFX_TRUE)
	{
		int length = 0;
		getObjIv(handle, GFX_SHADER_INFO_LOG_LENGTH, &length);

		std::string error(length, 0);
		getObjInfoLog(handle, length, &length, &error[0]);

		printf("OpenGL %s failure:\n%s\n", operation, error.c_str());
		return false;
	}
	return true;
}

bool check_shader(GFX_HANDLE handle) {
	const RenderingContext* context = RenderingContext::GetInstance();
	return _check_status(handle, GFX_COMPILE_STATUS, "shader compilation", context->QueryShaderParameter, context->GetShaderInfoLog);
}

bool check_program(GFX_HANDLE handle) {
	const RenderingContext* context = RenderingContext::GetInstance();
	return _check_status(handle, GFX_LINK_STATUS, "program link", context->QueryProgramParameter, context->GetProgramInfoLog);
}

GFX_HANDLE create_shader(uint32_t type, const char* source)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	GFX_HANDLE id = context->CreateShader(type);
	context->SetShaderSource(id, 1, &source, 0);
	context->CompileShader(id);
	return id;
}

void destroy_shader(GFX_HANDLE handle)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	if (context->IsShader(handle) == GFX_TRUE)
	{
		context->DeleteShader(handle);
	}
}

void destroy_program(GFX_HANDLE handle)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	if (context->IsProgram(handle) == GFX_TRUE)
	{
		context->DeleteProgram(handle);
	}
}

Shader::Shader()
{
	m_Handle = 0;
}

Shader::Shader(const char* vsrc, const char* fsrc, void(*pfn_bind)(GFX_HANDLE))
{
	Load(vsrc, fsrc, pfn_bind);
}

Shader::~Shader()
{
	destroy_program(m_Handle);
}

void Shader::Load(const char* vsrc, const char* fsrc, void(*pfn_bind)(GFX_HANDLE))
{
	const RenderingContext* context = RenderingContext::GetInstance();

	GFX_HANDLE vshdr = create_shader(GFX_VERTEX_SHADER, vsrc);
	GFX_HANDLE fshdr = create_shader(GFX_FRAGMENT_SHADER, fsrc);

	if (!check_shader(vshdr) || !check_shader(fshdr))
	{
		goto ERROR2;
	}

	m_Handle = context->CreateProgram();
	context->AttachShader(m_Handle, vshdr);
	context->AttachShader(m_Handle, fshdr);
	pfn_bind(m_Handle);
	context->LinkProgram(m_Handle);

	if (!check_program(m_Handle))
	{
		goto ERROR1;
	}

	context->DetachShader(m_Handle, vshdr);
	context->DetachShader(m_Handle, fshdr);
	destroy_shader(vshdr);
	destroy_shader(fshdr);

	return;

ERROR1:
	destroy_program(m_Handle);

ERROR2:
	destroy_shader(vshdr);
	destroy_shader(fshdr);
	
	printf("Error loading shader\n");
}

void Shader::Bind() const
{
	RenderingContext::GetInstance()->BindProgram(m_Handle);
}

int Shader::GetAttributeLocation(const char* name)
{
	int id = RenderingContext::GetInstance()->GetAttributeLocation(m_Handle, name);
	if (id < 0)
	{
		printf("Error: Attribute %s not found in shader\n", name);
	}
	return id;
}

int Shader::GetUniformLocation(const char* name)
{
	int id = RenderingContext::GetInstance()->GetUniformLocation(m_Handle, name);
	if (id < 0)
	{
		printf("Error: Uniform %s not found in shader\n", name);
	}
	return id;
}
