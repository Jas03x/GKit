#include <gk/graphics.hpp>

#include <cassert>
#include <cstdio>

#if defined(_WIN32)
	#include <WinDef.h>
	#include <Windows.h>
	#include <wingdi.h> // for wglGet* functions
	#include <libloaderapi.h>
#elif defined(__APPLE__)
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl3.h>
#else
    #include <GL/gl.h>
    #include <GL/glx.h>
#endif

#if defined(_WIN32)
    void* __GetFunctionAddress(const char* name, HMODULE module, bool* status)
    {
        #define INVALID_PFN(p) ((p == (void*) 0x0) || (p == (void*) 0x1) || (p == (void*) 0x2) || (p == (void*) 0x3) || (p == (void*) -1))

        void* func = (void*) wglGetProcAddress(name);
        if(INVALID_PFN(func))
        {
            func = (void*) GetProcAddress(module, name);
        }

        if(INVALID_PFN(func))
        {
            printf("Error: Could not find the function address of %s\n", name);
            *status = false;
        }
        
        return func;
    }
#elif defined(__linux__) || defined(__unix__)
    void* __GetFunctionAddress(const char* name, bool* status)
    {
        void* func = (void*) glXGetProcAddressARB((const GLubyte*) name);

        if(func == nullptr)
        {
            printf("error: could not find the function address of opengl function \"%s\"\n", name);
            *status = false;
        }

        return func;
    }
#endif

RenderingContext* RenderingContext::Instance = nullptr;

bool RenderingContext::Initialize()
{
    #ifdef _WIN32
        #define GetFunctionAddress(name, status) __GetFunctionAddress(#name, module, status)

        assert(wglGetCurrentContext() != 0);

        HMODULE module = LoadLibraryA("opengl32.dll");
        assert(module != 0);
    #elif defined(__APPLE__)
        #define GetFunctionAddress(name, status) name
    #elif defined(__linux__) || defined(__unix__)
        #define GetFunctionAddress(name, status) __GetFunctionAddress(#name, (status))
    #endif

    bool status = true;

    if(status) ActivateTextureSlot          = reinterpret_cast<GFX_PFN_ActiveTexture>            (GetFunctionAddress(glActiveTexture, &status));
    if(status) AttachShader                 = reinterpret_cast<GFX_PFN_AttachShader>             (GetFunctionAddress(glAttachShader, &status));
    if(status) DetachShader                 = reinterpret_cast<GFX_PFN_DetachShader>             (GetFunctionAddress(glDetachShader, &status));
    if(status) SetAttributeLocation         = reinterpret_cast<GFX_PFN_BindAttribLocation>       (GetFunctionAddress(glBindAttribLocation, &status));
    if(status) BindAllocation               = reinterpret_cast<GFX_PFN_BindBuffer>               (GetFunctionAddress(glBindBuffer, &status));
    if(status) SetFragmentLocation          = reinterpret_cast<GFX_PFN_BindFragDataLocation>     (GetFunctionAddress(glBindFragDataLocation, &status));
    if(status) BindFramebuffer              = reinterpret_cast<GFX_PFN_BindFramebuffer>          (GetFunctionAddress(glBindFramebuffer, &status));
    if(status) BindRenderbuffer             = reinterpret_cast<GFX_PFN_BindRenderbuffer>         (GetFunctionAddress(glBindRenderbuffer, &status));
    if(status) BindTexture                  = reinterpret_cast<GFX_PFN_BindTexture>              (GetFunctionAddress(glBindTexture, &status));
    if(status) BindVertexArray              = reinterpret_cast<GFX_PFN_BindVertexArray>          (GetFunctionAddress(glBindVertexArray, &status));
    if(status) SetBlendFunction             = reinterpret_cast<GFX_PFN_BlendFunc>                (GetFunctionAddress(glBlendFunc, &status));
    if(status) CreateBuffer                 = reinterpret_cast<GFX_PFN_BufferData>               (GetFunctionAddress(glBufferData, &status));
    if(status) UpdateBuffer                 = reinterpret_cast<GFX_PFN_BufferSubData>            (GetFunctionAddress(glBufferSubData, &status));
    if(status) ClearFramebuffer             = reinterpret_cast<GFX_PFN_Clear>                    (GetFunctionAddress(glClear, &status));
    if(status) SetClearColor                = reinterpret_cast<GFX_PFN_ClearColor>               (GetFunctionAddress(glClearColor, &status));
    if(status) CompileShader                = reinterpret_cast<GFX_PFN_CompileShader>            (GetFunctionAddress(glCompileShader, &status));
    if(status) CreateShader                 = reinterpret_cast<GFX_PFN_CreateShader>             (GetFunctionAddress(glCreateShader, &status));
    if(status) CreateProgram                = reinterpret_cast<GFX_PFN_CreateProgram>            (GetFunctionAddress(glCreateProgram, &status));
    if(status) DeleteAllocations            = reinterpret_cast<GFX_PFN_DeleteBuffers>            (GetFunctionAddress(glDeleteBuffers, &status));
    if(status) DeleteFramebuffers           = reinterpret_cast<GFX_PFN_DeleteFramebuffers>       (GetFunctionAddress(glDeleteFramebuffers, &status));
    if(status) DeleteProgram                = reinterpret_cast<GFX_PFN_DeleteProgram>            (GetFunctionAddress(glDeleteProgram, &status));
    if(status) DeleteRenderbuffers          = reinterpret_cast<GFX_PFN_DeleteRenderbuffers>      (GetFunctionAddress(glDeleteRenderbuffers, &status));
    if(status) DeleteShader                 = reinterpret_cast<GFX_PFN_DeleteShader>             (GetFunctionAddress(glDeleteShader, &status));
    if(status) DeleteTextures               = reinterpret_cast<GFX_PFN_DeleteTextures>           (GetFunctionAddress(glDeleteTextures, &status));
    if(status) DeleteVertexArrays           = reinterpret_cast<GFX_PFN_DeleteVertexArrays>       (GetFunctionAddress(glDeleteVertexArrays, &status));
    if(status) SetDepthFunction             = reinterpret_cast<GFX_PFN_DepthFunc>                (GetFunctionAddress(glDepthFunc, &status));
    if(status) SetDepthMask                 = reinterpret_cast<GFX_PFN_DepthMask>                (GetFunctionAddress(glDepthMask, &status));
    if(status) Disable                      = reinterpret_cast<GFX_PFN_Disable>                  (GetFunctionAddress(glDisable, &status));
    if(status) BindDrawBuffer               = reinterpret_cast<GFX_PFN_DrawBuffer>               (GetFunctionAddress(glDrawBuffer, &status));
    if(status) BindDrawBuffers              = reinterpret_cast<GFX_PFN_DrawBuffers>              (GetFunctionAddress(glDrawBuffers, &status));
    if(status) DrawArray                    = reinterpret_cast<GFX_PFN_DrawArrays>               (GetFunctionAddress(glDrawArrays, &status));
    if(status) DrawArrayInstanced           = reinterpret_cast<GFX_PFN_DrawArraysInstanced>      (GetFunctionAddress(glDrawArraysInstanced, &status));
    if(status) DrawElements                 = reinterpret_cast<GFX_PFN_DrawElements>             (GetFunctionAddress(glDrawElements, &status));
    if(status) DrawElementsInstanced        = reinterpret_cast<GFX_PFN_DrawElementsInstanced>    (GetFunctionAddress(glDrawElementsInstanced, &status));
    if(status) Enable                       = reinterpret_cast<GFX_PFN_Enable>                   (GetFunctionAddress(glEnable, &status));
    if(status) EnableVertexAttribute        = reinterpret_cast<GFX_PFN_EnableVertexAttrib>       (GetFunctionAddress(glEnableVertexAttribArray, &status));
    if(status) DisableVertexAttribute       = reinterpret_cast<GFX_PFN_DisableVertexAttrib>      (GetFunctionAddress(glDisableVertexAttribArray, &status));
    if(status) SetFramebufferRenderbuffer   = reinterpret_cast<GFX_PFN_FramebufferRenderbuffer>  (GetFunctionAddress(glFramebufferRenderbuffer, &status));
    if(status) SetFramebufferTexture        = reinterpret_cast<GFX_PFN_FramebufferTexture>       (GetFunctionAddress(glFramebufferTexture, &status));
    if(status) SetFramebufferTexture2D      = reinterpret_cast<GFX_PFN_FramebufferTexture2D>     (GetFunctionAddress(glFramebufferTexture2D, &status));
    if(status) CreateAllocations            = reinterpret_cast<GFX_PFN_GenBuffers>               (GetFunctionAddress(glGenBuffers, &status));
    if(status) CreateFramebuffers           = reinterpret_cast<GFX_PFN_GenFramebuffers>          (GetFunctionAddress(glGenFramebuffers, &status));
    if(status) CreateRenderbuffers          = reinterpret_cast<GFX_PFN_GenRenderbuffers>         (GetFunctionAddress(glGenRenderbuffers, &status));
    if(status) CreateTextures               = reinterpret_cast<GFX_PFN_GenTextures>              (GetFunctionAddress(glGenTextures, &status));
    if(status) CreateVertexArrays           = reinterpret_cast<GFX_PFN_GenVertexArrays>          (GetFunctionAddress(glGenVertexArrays, &status));
    if(status) GetAttributeLocation         = reinterpret_cast<GFX_PFN_GetAttribLocation>        (GetFunctionAddress(glGetAttribLocation, &status));
    if(status) GetUniformLocation           = reinterpret_cast<GFX_PFN_GetUniformLocation>       (GetFunctionAddress(glGetUniformLocation, &status));
    if(status) QueryProgramParameter        = reinterpret_cast<GFX_PFN_GetProgramIv>             (GetFunctionAddress(glGetProgramiv, &status));
    if(status) QueryShaderParameter         = reinterpret_cast<GFX_PFN_GetShaderIv>              (GetFunctionAddress(glGetShaderiv, &status));
    if(status) GetShaderInfoLog             = reinterpret_cast<GFX_PFN_GetShaderInfoLog>         (GetFunctionAddress(glGetShaderInfoLog, &status));
    if(status) GetProgramInfoLog            = reinterpret_cast<GFX_PFN_GetProgramInfoLog>        (GetFunctionAddress(glGetProgramInfoLog, &status));
    if(status) GetFramebufferStatus         = reinterpret_cast<GFX_PFN_CheckFramebufferStatus>   (GetFunctionAddress(glCheckFramebufferStatus, &status));
    if(status) IsAllocation                 = reinterpret_cast<GFX_PFN_IsBuffer>                 (GetFunctionAddress(glIsBuffer, &status));
    if(status) IsFramebuffer                = reinterpret_cast<GFX_PFN_IsFramebuffer>            (GetFunctionAddress(glIsFramebuffer, &status));
    if(status) IsProgram                    = reinterpret_cast<GFX_PFN_IsProgram>                (GetFunctionAddress(glIsProgram, &status));
    if(status) IsRenderbuffer               = reinterpret_cast<GFX_PFN_IsRenderbuffer>           (GetFunctionAddress(glIsRenderbuffer, &status));
    if(status) IsShader                     = reinterpret_cast<GFX_PFN_IsShader>                 (GetFunctionAddress(glIsShader, &status));
    if(status) IsTexture                    = reinterpret_cast<GFX_PFN_IsTexture>                (GetFunctionAddress(glIsTexture, &status));
    if(status) IsVertexArray                = reinterpret_cast<GFX_PFN_IsVertexArray>            (GetFunctionAddress(glIsVertexArray, &status));
    if(status) RenderbufferStorage          = reinterpret_cast<GFX_PFN_RenderbufferStorage>      (GetFunctionAddress(glRenderbufferStorage, &status));
    if(status) CreateTexture2D              = reinterpret_cast<GFX_PFN_TexImage2D>               (GetFunctionAddress(glTexImage2D, &status));
    if(status) SetTextureParameter          = reinterpret_cast<GFX_PFN_TexParameterI>            (GetFunctionAddress(glTexParameteri, &status));
    if(status) LoadConstant1F               = reinterpret_cast<GFX_PFN_Uniform1F>                (GetFunctionAddress(glUniform1f, &status));
    if(status) LoadConstant2F               = reinterpret_cast<GFX_PFN_Uniform2F>                (GetFunctionAddress(glUniform2f, &status));
    if(status) LoadConstant3F               = reinterpret_cast<GFX_PFN_Uniform3F>                (GetFunctionAddress(glUniform3f, &status));
    if(status) LoadConstant4F               = reinterpret_cast<GFX_PFN_Uniform4F>                (GetFunctionAddress(glUniform4f, &status));
    if(status) LoadConstant1I               = reinterpret_cast<GFX_PFN_Uniform1I>                (GetFunctionAddress(glUniform1i, &status));
    if(status) LoadConstant2I               = reinterpret_cast<GFX_PFN_Uniform2I>                (GetFunctionAddress(glUniform2i, &status));
    if(status) LoadConstant3I               = reinterpret_cast<GFX_PFN_Uniform3I>                (GetFunctionAddress(glUniform3i, &status));
    if(status) LoadConstant4I               = reinterpret_cast<GFX_PFN_Uniform4I>                (GetFunctionAddress(glUniform4i, &status));
    if(status) LoadConstant1U               = reinterpret_cast<GFX_PFN_Uniform1UI>               (GetFunctionAddress(glUniform1ui, &status));
    if(status) LoadConstant2U               = reinterpret_cast<GFX_PFN_Uniform2UI>               (GetFunctionAddress(glUniform2ui, &status));
    if(status) LoadConstant3U               = reinterpret_cast<GFX_PFN_Uniform3UI>               (GetFunctionAddress(glUniform3ui, &status));
    if(status) LoadConstant4U               = reinterpret_cast<GFX_PFN_Uniform4UI>               (GetFunctionAddress(glUniform4ui, &status));
    if(status) LoadConstantArray1F          = reinterpret_cast<GFX_PFN_Uniform1FV>               (GetFunctionAddress(glUniform1fv, &status));
    if(status) LoadConstantArray2F          = reinterpret_cast<GFX_PFN_Uniform2FV>               (GetFunctionAddress(glUniform2fv, &status));
    if(status) LoadConstantArray3F          = reinterpret_cast<GFX_PFN_Uniform3FV>               (GetFunctionAddress(glUniform3fv, &status));
    if(status) LoadConstantArray4F          = reinterpret_cast<GFX_PFN_Uniform4FV>               (GetFunctionAddress(glUniform4fv, &status));
    if(status) LoadConstantArray1I          = reinterpret_cast<GFX_PFN_Uniform1IV>               (GetFunctionAddress(glUniform1iv, &status));
    if(status) LoadConstantArray2I          = reinterpret_cast<GFX_PFN_Uniform2IV>               (GetFunctionAddress(glUniform2iv, &status));
    if(status) LoadConstantArray3I          = reinterpret_cast<GFX_PFN_Uniform3IV>               (GetFunctionAddress(glUniform3iv, &status));
    if(status) LoadConstantArray4I          = reinterpret_cast<GFX_PFN_Uniform4IV>               (GetFunctionAddress(glUniform4iv, &status));
    if(status) LoadConstantArray1U          = reinterpret_cast<GFX_PFN_Uniform1UIV>              (GetFunctionAddress(glUniform1uiv, &status));
    if(status) LoadConstantArray2U          = reinterpret_cast<GFX_PFN_Uniform2UIV>              (GetFunctionAddress(glUniform2uiv, &status));
    if(status) LoadConstantArray3U          = reinterpret_cast<GFX_PFN_Uniform3UIV>              (GetFunctionAddress(glUniform3uiv, &status));
    if(status) LoadConstantArray4U          = reinterpret_cast<GFX_PFN_Uniform4UIV>              (GetFunctionAddress(glUniform4uiv, &status));
    if(status) LoadConstantMatrix2F         = reinterpret_cast<GFX_PFN_UniformMatrix2FV>         (GetFunctionAddress(glUniformMatrix2fv, &status));
    if(status) LoadConstantMatrix3F         = reinterpret_cast<GFX_PFN_UniformMatrix3FV>         (GetFunctionAddress(glUniformMatrix3fv, &status));
    if(status) LoadConstantMatrix4F         = reinterpret_cast<GFX_PFN_UniformMatrix4FV>         (GetFunctionAddress(glUniformMatrix4fv, &status));
    if(status) SetVertexAttributeLayoutI    = reinterpret_cast<GFX_PFN_VertexAttribIPointer>     (GetFunctionAddress(glVertexAttribIPointer, &status));
    if(status) SetVertexAttributeLayoutF    = reinterpret_cast<GFX_PFN_VertexAttribFPointer>     (GetFunctionAddress(glVertexAttribPointer, &status));
    if(status) SetViewport                  = reinterpret_cast<GFX_PFN_Viewport>                 (GetFunctionAddress(glViewport, &status));
    if(status) SetShaderSource              = reinterpret_cast<GFX_PFN_ShaderSource>             (GetFunctionAddress(glShaderSource, &status));
    if(status) LinkProgram                  = reinterpret_cast<GFX_PFN_LinkProgram>              (GetFunctionAddress(glLinkProgram, &status));
    if(status) DrawElementArrays            = reinterpret_cast<GFX_PFN_MultiDrawElements>        (GetFunctionAddress(glMultiDrawElements, &status));
    if(status) SetPolygonMode               = reinterpret_cast<GFX_PFN_PolygonMode>              (GetFunctionAddress(glPolygonMode, &status));
    if(status) BindProgram                  = reinterpret_cast<GFX_PFN_UseProgram>               (GetFunctionAddress(glUseProgram, &status));
    if(status) ErrorCheck                   = reinterpret_cast<GFX_PFN_GetError>                 (GetFunctionAddress(glGetError, &status));
    if(status) SetDepthRange                = reinterpret_cast<GFX_PFN_DepthRange>               (GetFunctionAddress(glDepthRange, &status));
	
	if (!status)
	{
		printf("Error loading OpenGL functions\n");
		return false;
	}

    #ifdef _WIN32
    FreeLibrary(module);
    #endif

	return true;
}

RenderingContext::~RenderingContext()
{

}

bool RenderingContext::CreateInstance()
{
    assert(Instance == nullptr);

    if(Instance == nullptr)
    {
        Instance = new RenderingContext();
		return Instance->Initialize();
    }

	return false;
}

bool RenderingContext::DeleteInstance()
{
	assert(Instance != nullptr);

    if(Instance != nullptr)
    {
        delete Instance;
        Instance = nullptr;

		return true;
    }

	return false;
}

const RenderingContext* RenderingContext::GetInstance()
{
	assert(Instance != nullptr);
    return Instance;
}
