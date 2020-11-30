#ifndef GK_CONTEXT_H
#define GK_CONTEXT_H

#include <cstdint>

enum KEY_CODE
{
	KEY_UNKNOWN	= 0x00,
	KEY_UP		= 0x01,
	KEY_DOWN	= 0x02,
	KEY_LEFT	= 0x03,
	KEY_RIGHT	= 0x04,
	KEY_W		= 0x05,
	KEY_A		= 0x06,
	KEY_S		= 0x07,
	KEY_D		= 0x08,
	KEY_Q		= 0x09,
	KEY_E		= 0x0A,
	KEY_T       = 0x0B,
	KEY_U       = 0x0C,
	KEY_Y       = 0x0D,
	KEY_RETURN	= 0x0E,
	KEY_ESCAPE  = 0x0F,
	KEY_SPACE   = 0x10,
	KEY_COUNT   = 0x11
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

struct MouseState
{
	int  x_offset;
	int  y_offset;
	bool l_button;
	bool m_button;
	bool r_button;
};

enum MOUSE_MODE : uint8_t
{
	DISABLED = 0,
	ABSOLUTE = 1,
	RELATIVE = 2
};

class Context
{
public:
	static bool CreateInstance(const char* title, unsigned int width, unsigned int height);
	static void DeleteInstance();

	static void Update();
	static bool PollEvent(Event& e);
	
	static MOUSE_MODE GetMouseMode();
	static bool       SetMouseMode(MOUSE_MODE mode);

	static void GetMouseState(MouseState& state);

	static uint32_t GetTime();
	static bool GetKeyState(KEY_CODE code);
};

#endif // GK_CONTEXT_H
