#ifndef GK_GRAPHICS_H
#define GK_GRAPHICS_H

#include <gk/data_types.hpp>

// Graphics Layer

typedef unsigned int GFX_HANDLE;

enum
{
    GFX_NULL = 0
};

enum GFX_TEXTURE_SLOT
{
    GFX_TEXTURE_SLOT0 = 0x84C0
};

enum GFX_COLOR
{
    GFX_RED             = 0x1903,
	GFX_RGB             = 0x1907,
	GFX_RGBA            = 0x1908,
	GFX_DEPTH_16        = 0x81A5,
	GFX_DEPTH_24        = 0x81A6,
	GFX_DEPTH_32        = 0x81A7,
	GFX_DEPTH_COMPONENT = 0x1902
};

enum GFX_TYPE
{
    GFX_TYPE_BYTE            = 0x1400,
    GFX_TYPE_UNSIGNED_BYTE   = 0x1401,
	GFX_TYPE_SHORT           = 0x1402,
	GFX_TYPE_UNSIGNED_SHORT  = 0x1403,
	GFX_TYPE_INT             = 0x1404,
	GFX_TYPE_UNSIGNED_INT    = 0x1405,
	GFX_TYPE_FLOAT           = 0x1406
};

enum GFX_BOOL
{
	GFX_FALSE = 0x0,
	GFX_TRUE  = 0x1
};

enum GFX_SETTING
{
	GFX_POINT_SIZE	 = 0x8642,
	GFX_FACE_CULL	 = 0x0B44,
	GFX_DEPTH_TEST   = 0x0B71,
	GFX_STENCIL_TEST = 0x0B90,
	GFX_BLEND        = 0x0BE2
};

enum GFX_BLEND_OP
{
	GFX_BLEND_ONE                 = 0x0001,
	GFX_BLEND_SRC_ALPHA           = 0x0302,
	GFX_BLEND_ONE_MINUS_SRC_ALPHA = 0x0303,
	GFX_BLEND_DST_ALPHA           = 0x0304
};

enum GFX_BUFFER
{
	GFX_DEPTH_BUFFER_BIT    = 0x00000100,
	GFX_STENCIL_BUFFER_BIT  = 0x00000400,
	GFX_COLOR_BUFFER_BIT    = 0x00004000
};

enum GFX_ATTACHMENT
{
	GFX_COLOR_ATTACHMENT0   = 0x8CE0,
	GFX_DEPTH_ATTACHMENT    = 0x8D00
};

enum GFX_BUFFER_USAGE
{
	GFX_STREAM_DRAW  = 0x88E0,
	GFX_STREAM_READ  = 0x88E1,
	GFX_STREAM_COPY  = 0x88E2,
	GFX_STATIC_DRAW  = 0x88E4,
	GFX_STATIC_READ  = 0x88E5,
	GFX_STATIC_COPY  = 0x88E6,
	GFX_DYNAMIC_DRAW = 0x88E8,
	GFX_DYNAMIC_READ = 0x88E9,
	GFX_DYNAMIC_COPY = 0x88EA
};

enum GFX_OBJECT
{
	GFX_ARRAY_BUFFER	= 0x8892,
	GFX_ELEMENT_BUFFER	= 0x8893,
	GFX_TEXTURE_2D		= 0x0DE1,
	GFX_RENDERBUFFER	= 0x8D41,
	GFX_FRAMEBUFFER		= 0x8D40,
	GFX_FRAGMENT_SHADER	= 0x8B30,
	GFX_VERTEX_SHADER	= 0x8B31,
};

enum GFX_TEXTURE
{
	GFX_TEXTURE_CUBE_MAP = 0x8513,
	GFX_TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515,
	GFX_TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516,
	GFX_TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517,
	GFX_TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518,
	GFX_TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519,
	GFX_TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A
};

enum GFX_SHADER_INFO
{
    GFX_COMPILE_STATUS         = 0x8B81,
	GFX_LINK_STATUS            = 0x8B82,
	GFX_SHADER_INFO_LOG_LENGTH = 0x8B84
};

enum GFX_FRAMEBUFFER_INFO
{
    GFX_COMPLETE_FRAMEBUFFER = 0x8CD5
};

enum GFX_DRAW_MODE
{
    GFX_POINTS    = 0x0000,
    GFX_LINES     = 0x0001,
	GFX_TRIANGLES = 0x0004
};

enum GFX_TEXTURE_PARAMETER
{
	GFX_TEXTURE_MAG_FILTER      = 0x2800,
	GFX_TEXTURE_MIN_FILTER      = 0x2801,
	GFX_TEXTURE_WRAP_S          = 0x2802,
	GFX_TEXTURE_WRAP_T          = 0x2803,
	GFX_TEXTURE_WRAP_R          = 0x8072,
	GFX_NEAREST                 = 0x2600,
	GFX_LINEAR                  = 0x2601,
	GFX_NEAREST_MIPMAP_NEAREST  = 0x2700,
	GFX_LINEAR_MIPMAP_NEAREST   = 0x2701,
	GFX_NEAREST_MIPMAP_LINEAR   = 0x2702,
	GFX_LINEAR_MIPMAP_LINEAR    = 0x2703,
	GFX_CLAMP_TO_EDGE           = 0x812F
};

enum GFX_DEPTH_FUNCTION
{
    GFX_NEVER    = 0x0200,
    GFX_LESS     = 0x0201,
    GFX_EQUAL    = 0x0202,
    GFX_LEQUAL   = 0x0203,
    GFX_GREATER  = 0x0204,
    GFX_NOTEQUAL = 0x0205,
    GFX_GEQUAL   = 0x0206,
    GFX_ALWAYS   = 0x0207
};

typedef void     (*GFX_PFN_ActiveTexture)(uint32_t);
typedef void     (*GFX_PFN_AttachShader)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_DetachShader)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_BindAttribLocation)(uint32_t, uint32_t, const char*);
typedef void     (*GFX_PFN_BindBuffer)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_BindFragDataLocation)(uint32_t, uint32_t, const char*);
typedef void     (*GFX_PFN_BindFramebuffer)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_BindRenderbuffer)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_BindTexture)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_BindVertexArray)(uint32_t);
typedef void     (*GFX_PFN_BlendFunc)(uint32_t, uint32_t);
typedef void     (*GFX_PFN_BufferData)(uint32_t, int64_t, const void*, uint32_t);
typedef void     (*GFX_PFN_BufferSubData)(uint32_t, int64_t, int64_t, const void*);
typedef void     (*GFX_PFN_Clear)(uint32_t);
typedef void     (*GFX_PFN_ClearColor)(float, float, float, float);
typedef void     (*GFX_PFN_CompileShader)(uint32_t);
typedef uint32_t (*GFX_PFN_CreateShader)(uint32_t);
typedef uint32_t (*GFX_PFN_CreateProgram)(void);
typedef void     (*GFX_PFN_DeleteBuffers)(int32_t, const uint32_t*);
typedef void     (*GFX_PFN_DeleteFramebuffers)(int32_t, const uint32_t*);
typedef void     (*GFX_PFN_DeleteProgram)(uint32_t);
typedef void     (*GFX_PFN_DeleteRenderbuffers)(int32_t, const uint32_t*);
typedef void     (*GFX_PFN_DeleteShader)(uint32_t);
typedef void     (*GFX_PFN_DeleteTextures)(int32_t, const uint32_t*);
typedef void     (*GFX_PFN_DeleteVertexArrays)(int32_t, const uint32_t*);
typedef void     (*GFX_PFN_DepthFunc)(uint32_t);
typedef void     (*GFX_PFN_DepthMask)(uint8_t);
typedef void     (*GFX_PFN_DepthRange)(double, double);
typedef void     (*GFX_PFN_Disable)(uint32_t);
typedef void     (*GFX_PFN_DrawBuffer)(uint32_t);
typedef void     (*GFX_PFN_DrawBuffers)(int32_t, const uint32_t*);
typedef void     (*GFX_PFN_DrawArrays)(uint32_t, int32_t, int32_t);
typedef void     (*GFX_PFN_DrawArraysInstanced)(uint32_t, int32_t, int32_t, int32_t);
typedef void     (*GFX_PFN_DrawElements)(uint32_t, int32_t, uint32_t, const void*);
typedef void     (*GFX_PFN_DrawElementsInstanced)(uint32_t, int32_t, uint32_t, const void*, int32_t);
typedef void     (*GFX_PFN_Enable)(uint32_t);
typedef void     (*GFX_PFN_EnableVertexAttrib)(uint32_t);
typedef void     (*GFX_PFN_DisableVertexAttrib)(uint32_t);
typedef void     (*GFX_PFN_FramebufferRenderbuffer)(uint32_t, uint32_t, uint32_t, uint32_t);
typedef void     (*GFX_PFN_FramebufferTexture)(uint32_t, uint32_t, uint32_t, int32_t);
typedef void     (*GFX_PFN_FramebufferTexture2D)(uint32_t, uint32_t, uint32_t, uint32_t, int32_t);
typedef void     (*GFX_PFN_GenBuffers)(int32_t, uint32_t*);
typedef void     (*GFX_PFN_GenFramebuffers)(int32_t, uint32_t*);
typedef void     (*GFX_PFN_GenTextures)(int32_t, uint32_t*);
typedef void     (*GFX_PFN_GenRenderbuffers)(int32_t, uint32_t*);
typedef void     (*GFX_PFN_GenVertexArrays)(int32_t, uint32_t*);
typedef uint32_t (*GFX_PFN_GetError)(void);
typedef int32_t  (*GFX_PFN_GetAttribLocation)(uint32_t, const char*);
typedef int32_t  (*GFX_PFN_GetUniformLocation)(uint32_t, const char*);
typedef void     (*GFX_PFN_GetProgramIv)(uint32_t, uint32_t, int32_t*);
typedef void     (*GFX_PFN_GetShaderIv)(uint32_t, uint32_t, int32_t*);
typedef void     (*GFX_PFN_GetShaderInfoLog)(uint32_t, int32_t, int32_t*, char*);
typedef void     (*GFX_PFN_GetProgramInfoLog)(uint32_t, int32_t, int32_t*, char*);
typedef uint32_t (*GFX_PFN_CheckFramebufferStatus)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsBuffer)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsFramebuffer)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsProgram)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsRenderbuffer)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsShader)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsTexture)(uint32_t);
typedef uint8_t  (*GFX_PFN_IsVertexArray)(uint32_t);
typedef void     (*GFX_PFN_LinkProgram)(uint32_t);
typedef void     (*GFX_PFN_RenderbufferStorage)(uint32_t, uint32_t, int32_t, int32_t);
typedef void     (*GFX_PFN_ShaderSource)(uint32_t, int32_t, const char* const*, const int32_t*);
typedef void     (*GFX_PFN_TexImage2D)(uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t, uint32_t, const void*);
typedef void     (*GFX_PFN_TexParameterI)(uint32_t, uint32_t, int32_t);
typedef void     (*GFX_PFN_Uniform1F)(int32_t, float);
typedef void     (*GFX_PFN_Uniform2F)(int32_t, float, float);
typedef void     (*GFX_PFN_Uniform3F)(int32_t, float, float, float);
typedef void     (*GFX_PFN_Uniform4F)(int32_t, float, float, float, float);
typedef void     (*GFX_PFN_Uniform1I)(int32_t, int32_t);
typedef void     (*GFX_PFN_Uniform2I)(int32_t, int32_t, int32_t);
typedef void     (*GFX_PFN_Uniform3I)(int32_t, int32_t, int32_t, int32_t);
typedef void     (*GFX_PFN_Uniform4I)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void     (*GFX_PFN_Uniform1UI)(int32_t, uint32_t);
typedef void     (*GFX_PFN_Uniform2UI)(int32_t, uint32_t, uint32_t);
typedef void     (*GFX_PFN_Uniform3UI)(int32_t, uint32_t, uint32_t, uint32_t);
typedef void     (*GFX_PFN_Uniform4UI)(int32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void     (*GFX_PFN_Uniform1FV)(int32_t, int32_t, const float*);
typedef void     (*GFX_PFN_Uniform2FV)(int32_t, int32_t, const float*);
typedef void     (*GFX_PFN_Uniform3FV)(int32_t, int32_t, const float*);
typedef void     (*GFX_PFN_Uniform4FV)(int32_t, int32_t, const float*);
typedef void     (*GFX_PFN_Uniform1IV)(int32_t, int32_t, const int32_t*);
typedef void     (*GFX_PFN_Uniform2IV)(int32_t, int32_t, const int32_t*);
typedef void     (*GFX_PFN_Uniform3IV)(int32_t, int32_t, const int32_t*);
typedef void     (*GFX_PFN_Uniform4IV)(int32_t, int32_t, const int32_t*);
typedef void     (*GFX_PFN_Uniform1UIV)(int32_t, int32_t, const uint32_t*);
typedef void     (*GFX_PFN_Uniform2UIV)(int32_t, int32_t, const uint32_t*);
typedef void     (*GFX_PFN_Uniform3UIV)(int32_t, int32_t, const uint32_t*);
typedef void     (*GFX_PFN_Uniform4UIV)(int32_t, int32_t, const uint32_t*);
typedef void     (*GFX_PFN_UniformMatrix2FV)(int32_t, int32_t, uint8_t, const float*);
typedef void     (*GFX_PFN_UniformMatrix3FV)(int32_t, int32_t, uint8_t, const float*);
typedef void     (*GFX_PFN_UniformMatrix4FV)(int32_t, int32_t, uint8_t, const float*);
typedef void     (*GFX_PFN_UseProgram)(uint32_t);
typedef void     (*GFX_PFN_VertexAttribFPointer)(uint32_t, int32_t, uint32_t, uint8_t, int32_t, const void*);
typedef void     (*GFX_PFN_VertexAttribIPointer)(uint32_t, int32_t, uint32_t, int32_t, const void*);
typedef void     (*GFX_PFN_Viewport)(int32_t, int32_t, int32_t, int32_t);

class RenderingContext
{
private:
    static RenderingContext* Instance;

public:
    GFX_PFN_ActiveTexture             ActivateTextureSlot;
    GFX_PFN_AttachShader              AttachShader;
    GFX_PFN_DetachShader              DetachShader;
    GFX_PFN_BindAttribLocation        SetAttributeLocation;
    GFX_PFN_BindBuffer                BindAllocation;
    GFX_PFN_BindFragDataLocation      SetFragmentLocation;
    GFX_PFN_BindFramebuffer           BindFramebuffer;
    GFX_PFN_BindRenderbuffer          BindRenderbuffer;
    GFX_PFN_BindTexture               BindTexture;
    GFX_PFN_BindVertexArray           BindVertexArray;
    GFX_PFN_BlendFunc                 SetBlendFunction;
    GFX_PFN_BufferData                CreateBuffer;
    GFX_PFN_BufferSubData             UpdateBuffer;
    GFX_PFN_Clear                     ClearFramebuffer;
    GFX_PFN_ClearColor                SetClearColor;
    GFX_PFN_CompileShader             CompileShader;
    GFX_PFN_CreateShader              CreateShader;
    GFX_PFN_CreateProgram             CreateProgram;
    GFX_PFN_DeleteBuffers             DeleteAllocations;
    GFX_PFN_DeleteFramebuffers        DeleteFramebuffers;
    GFX_PFN_DeleteProgram             DeleteProgram;
    GFX_PFN_DeleteRenderbuffers       DeleteRenderbuffers;
    GFX_PFN_DeleteShader              DeleteShader;
    GFX_PFN_DeleteTextures            DeleteTextures;
    GFX_PFN_DeleteVertexArrays        DeleteVertexArrays;
    GFX_PFN_DepthFunc                 SetDepthFunction;
    GFX_PFN_DepthMask                 SetDepthMask;
    GFX_PFN_Disable                   Disable;
    GFX_PFN_DrawBuffer                BindDrawBuffer;
    GFX_PFN_DrawBuffers               BindDrawBuffers;
    GFX_PFN_DrawArrays                DrawArray;
    GFX_PFN_DrawArraysInstanced       DrawArrayInstanced;
    GFX_PFN_DrawElements              DrawElements;
    GFX_PFN_DrawElementsInstanced     DrawElementsInstanced;
    GFX_PFN_Enable                    Enable;
    GFX_PFN_EnableVertexAttrib        EnableVertexAttribute;
    GFX_PFN_DisableVertexAttrib       DisableVertexAttribute;
    GFX_PFN_FramebufferRenderbuffer   SetFramebufferRenderbuffer;
    GFX_PFN_FramebufferTexture        SetFramebufferTexture;
    GFX_PFN_FramebufferTexture2D      SetFramebufferTexture2D;
    GFX_PFN_GenBuffers                CreateAllocations;
    GFX_PFN_GenFramebuffers           CreateFramebuffers;
    GFX_PFN_GenTextures               CreateTextures;
    GFX_PFN_GenRenderbuffers          CreateRenderbuffers;
    GFX_PFN_GenVertexArrays           CreateVertexArrays;
    GFX_PFN_GetError                  ErrorCheck;
    GFX_PFN_GetAttribLocation         GetAttributeLocation;
    GFX_PFN_GetUniformLocation        GetUniformLocation;
    GFX_PFN_GetProgramIv              QueryProgramParameter;
    GFX_PFN_GetShaderIv               QueryShaderParameter;
    GFX_PFN_GetShaderInfoLog          GetShaderInfoLog;
    GFX_PFN_GetProgramInfoLog         GetProgramInfoLog;
    GFX_PFN_CheckFramebufferStatus    GetFramebufferStatus;
    GFX_PFN_IsBuffer                  IsAllocation;
    GFX_PFN_IsFramebuffer             IsFramebuffer;
    GFX_PFN_IsProgram                 IsProgram;
    GFX_PFN_IsRenderbuffer            IsRenderbuffer;
    GFX_PFN_IsShader                  IsShader;
    GFX_PFN_IsTexture                 IsTexture;
    GFX_PFN_IsVertexArray             IsVertexArray;
    GFX_PFN_LinkProgram               LinkProgram;
    GFX_PFN_RenderbufferStorage       RenderbufferStorage;
    GFX_PFN_TexImage2D                CreateTexture2D;
    GFX_PFN_TexParameterI             SetTextureParameter;
    GFX_PFN_Uniform1F                 LoadConstant1F;
    GFX_PFN_Uniform2F                 LoadConstant2F;
    GFX_PFN_Uniform3F                 LoadConstant3F;
    GFX_PFN_Uniform4F                 LoadConstant4F;
    GFX_PFN_Uniform1I                 LoadConstant1I;
    GFX_PFN_Uniform2I                 LoadConstant2I;
    GFX_PFN_Uniform3I                 LoadConstant3I;
    GFX_PFN_Uniform4I                 LoadConstant4I;
    GFX_PFN_Uniform1UI                LoadConstant1U;
    GFX_PFN_Uniform2UI                LoadConstant2U;
    GFX_PFN_Uniform3UI                LoadConstant3U;
    GFX_PFN_Uniform4UI                LoadConstant4U;
    GFX_PFN_Uniform1FV                LoadConstantArray1F;
    GFX_PFN_Uniform2FV                LoadConstantArray2F;
    GFX_PFN_Uniform3FV                LoadConstantArray3F;
    GFX_PFN_Uniform4FV                LoadConstantArray4F;
    GFX_PFN_Uniform1IV                LoadConstantArray1I;
    GFX_PFN_Uniform2IV                LoadConstantArray2I;
    GFX_PFN_Uniform3IV                LoadConstantArray3I;
    GFX_PFN_Uniform4IV                LoadConstantArray4I;
    GFX_PFN_Uniform1UIV               LoadConstantArray1U;
    GFX_PFN_Uniform2UIV               LoadConstantArray2U;
    GFX_PFN_Uniform3UIV               LoadConstantArray3U;
    GFX_PFN_Uniform4UIV               LoadConstantArray4U;
    GFX_PFN_UniformMatrix2FV          LoadConstantMatrix2F;
    GFX_PFN_UniformMatrix3FV          LoadConstantMatrix3F;
    GFX_PFN_UniformMatrix4FV          LoadConstantMatrix4F;
    GFX_PFN_VertexAttribIPointer      SetVertexAttributeLayoutI;
    GFX_PFN_VertexAttribFPointer      SetVertexAttributeLayoutF;
    GFX_PFN_Viewport                  SetViewport;
    GFX_PFN_ShaderSource              SetShaderSource;
    GFX_PFN_UseProgram                BindProgram;
    GFX_PFN_DepthRange                SetDepthRange;

private:
	bool Initialize();
    ~RenderingContext();

public:
    static bool CreateInstance();
    static bool DeleteInstance();

    static const RenderingContext* GetInstance();
};

#endif // GK_GRAPHICS_H
