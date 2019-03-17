#ifndef GK_CONTEXT_H
#define GK_CONTEXT_H

#include <gk/data_types.hpp>

class Context
{
public:
	enum { KEYBOARD_LENGTH = 256 };

	enum
	{
		KEY_NULL	= 0x0,
		KEY_Q		= 0x1,
		KEY_W		= 0x2,
		KEY_E		= 0x3,
		KEY_A		= 0x4,
		KEY_S		= 0x5,
		KEY_D		= 0x6,
		KEY_UP		= 0x7,
		KEY_DOWN	= 0x8,
		KEY_LEFT	= 0x9,
		KEY_RIGHT	= 0xA,
		KEY_SPACE	= 0xB,
		KEY_ESCAPE	= 0xC
	};

private:
	// different platforms have different data:
	void* DATA;

public:
	static bool CreateInstance(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char* title);
	static bool DeleteInstance();

	static Context* GetInstance();

	static void*	GetPrivateData();

	static void		ProcessEvents();
	static bool		Running();
	static byte*    GetKeyboard();

	static void		Update();
};

#endif // GK_CONTEXT_H
