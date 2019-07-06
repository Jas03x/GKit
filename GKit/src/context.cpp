#include <gk/context.hpp>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <gk/context.hpp>
#include <gk/graphics.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class ContextImpl
{
private:
	SDL_Window*   m_Window;
	SDL_GLContext m_Context;

	uint16_t      m_KeyMap[SDL_NUM_SCANCODES];

public:
	ContextImpl(const char* title, unsigned int width, unsigned int height);
	~ContextImpl();

	void Update();
	bool PollEvent(Event& e);
};

void SDL_ERROR(const char* msg) {
	printf("%s:\n%s\n", msg, SDL_GetError());
}

#define SDL_ASSERT(b, e) if(!(b)) { SDL_ERROR(e); throw -1; }

ContextImpl::ContextImpl(const char* title, unsigned int width, unsigned int height)
{
	SDL_ASSERT(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "SDL initialization failure");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_Window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	SDL_ASSERT(m_Window != NULL, "SDL window creation failure");

	m_Context = SDL_GL_CreateContext(m_Window);
	SDL_ASSERT(m_Context != NULL, "SDL OpenGL context creation failure");

	memset(m_KeyMap, 0, sizeof(m_KeyMap));
	m_KeyMap[SDL_SCANCODE_UP]     = KEY_UP;
	m_KeyMap[SDL_SCANCODE_DOWN]   = KEY_DOWN;
	m_KeyMap[SDL_SCANCODE_LEFT]   = KEY_LEFT;
	m_KeyMap[SDL_SCANCODE_RIGHT]  = KEY_RIGHT;
	m_KeyMap[SDL_SCANCODE_W]	  = KEY_W;
	m_KeyMap[SDL_SCANCODE_A]	  = KEY_A;
	m_KeyMap[SDL_SCANCODE_S]	  = KEY_S;
	m_KeyMap[SDL_SCANCODE_D]	  = KEY_D;
	m_KeyMap[SDL_SCANCODE_Q]	  = KEY_Q;
	m_KeyMap[SDL_SCANCODE_E]	  = KEY_E;
	m_KeyMap[SDL_SCANCODE_RETURN] = KEY_RETURN;
	m_KeyMap[SDL_SCANCODE_ESCAPE] = KEY_ESCAPE;
}

ContextImpl::~ContextImpl()
{
	if (m_Context != nullptr) {
		SDL_GL_DeleteContext(m_Context);
	}

	if (m_Window != nullptr) {
		SDL_DestroyWindow(m_Window);
	}

	SDL_Quit();
}

void ContextImpl::Update()
{
	SDL_GL_SwapWindow(m_Window);
}

bool ContextImpl::PollEvent(Event& e)
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				e.type = EVENT_QUIT;

				return true;
			}
			case SDL_KEYUP:
			case SDL_KEYDOWN:
			{
				e.key_args.key = m_KeyMap[event.key.keysym.scancode];
				e.type = EVENT_KEY_INPUT;
				e.key_args.action = (event.type == SDL_KEYDOWN) ? ACTION_KEY_PRESSED : ACTION_KEY_RELEASED;

				return true;
			}
			default: break;
		}
	}
	
	return false;
}

static ContextImpl* g_ContextImpl = nullptr;

bool Context::CreateInstance(const char* title, unsigned int width, unsigned int height)
{
	assert(g_ContextImpl == nullptr);

	g_ContextImpl = new ContextImpl(title, width, height);

	return (g_ContextImpl != nullptr);
}

void Context::DeleteInstance()
{
	assert(g_ContextImpl != nullptr);

	if (g_ContextImpl != nullptr)
	{
		delete g_ContextImpl;
		g_ContextImpl = nullptr;
	}
}

void Context::Update()
{
	assert(g_ContextImpl != nullptr);

	g_ContextImpl->Update();
}

bool Context::PollEvent(Event& e)
{
	assert(g_ContextImpl != nullptr);

	return g_ContextImpl->PollEvent(e);
}

uint32_t Context::GetTime()
{
	return SDL_GetTicks();
}
