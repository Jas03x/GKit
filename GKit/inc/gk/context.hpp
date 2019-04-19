#ifndef GK_CONTEXT_H
#define GK_CONTEXT_H

#include <stdint.h>

namespace INPUT
{
	enum
	{
		KEY_UNKNOWN	= 0x0,
		KEY_UP		= 0x1,
		KEY_DOWN	= 0x2,
		KEY_LEFT	= 0x3,
		KEY_RIGHT	= 0x4,
		KEY_W		= 0x5,
		KEY_A		= 0x6,
		KEY_S		= 0x7,
		KEY_D		= 0x8,
		KEY_Q		= 0x9,
		KEY_E		= 0xA,
		KEY_RETURN	= 0xB,
		KEY_ESCAPE  = 0xC,
		KEY_COUNT   = 0xD
	};

	enum
	{
		KEY_PRESSED  = 0x1,
		KEY_RELEASED = 0x2
	};
}

enum EVENT_TYPE
{
	UNKNOWN   = 0x0,
	QUIT      = 0x1,
	KEY_INPUT = 0x2
};

struct Event
{
	uint8_t Type;

	union
	{
		struct
		{
			uint16_t Key;
			uint8_t  Action;
		} KeyArgs;
	};
};

class Context
{
public:
	static bool CreateInstance(const char* title, unsigned int width, unsigned int height);
	static void DeleteInstance();

	static uint32_t GetTime();

	static void Update();
	static bool PollEvent(Event& e);
};

#endif // GK_CONTEXT_H
