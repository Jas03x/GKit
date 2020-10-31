#ifndef GK_CONTEXT_H
#define GK_CONTEXT_H

#include <cstdint>

enum KEY_CODE
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
	KEY_T       = 0xB,
	KEY_RETURN	= 0xC,
	KEY_ESCAPE  = 0xD,
	KEY_SPACE   = 0xE,
	KEY_COUNT   = 0xF
};

enum EVENT_TYPE
{
	EVENT_UNKNOWN   = 0x0,
	EVENT_QUIT      = 0x1
};

struct Event
{
	uint8_t type;
};

class Context
{
public:
	static bool CreateInstance(const char* title, unsigned int width, unsigned int height);
	static void DeleteInstance();

	static void Update();
	static bool PollEvent(Event& e);

	static uint32_t GetTime();
	static bool GetKeyState(KEY_CODE code);
};

#endif // GK_CONTEXT_H
