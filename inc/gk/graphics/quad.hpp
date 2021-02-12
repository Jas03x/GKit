#ifndef GK_QUAD_H
#define GK_QUAD_H

#include <gk/graphics.hpp>

class Quad
{
private:
	static Quad* Instance;
    
private:
	GFX_HANDLE m_VAO;
	GFX_HANDLE m_VBO;

private:
	Quad();
	~Quad();

public:
	typedef enum
	{
		VERTEX = 0
	} VertexAttributes;

public:
	static Quad* CreateInstance();
	static void DeleteInstance();
    
    static Quad* GetInstance();

public:
	void Bind();
	unsigned int GetVertexCount();
};

#endif // GK_QUAD_H
