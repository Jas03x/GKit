#include <gk/context.hpp>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <gk/context.hpp>
#include <gk/graphics.hpp>

#define HAVE_M_PI
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

SDL_Window* g_Window = nullptr;
SDL_GLContext g_Context = nullptr;

const uint8_t* g_Keyboard = nullptr;
const uint16_t g_KeyMap[KEY_COUNT] =
{
	0x0,
	SDL_SCANCODE_UP,
	SDL_SCANCODE_DOWN,
	SDL_SCANCODE_LEFT,
	SDL_SCANCODE_RIGHT,
	SDL_SCANCODE_W,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_Q,
	SDL_SCANCODE_E,
	SDL_SCANCODE_T,
	SDL_SCANCODE_RETURN,
	SDL_SCANCODE_ESCAPE,
	SDL_SCANCODE_SPACE
};

void SDL_ERROR(const char* msg) {
	printf("%s:\n%s\n", msg, SDL_GetError());
}


bool Context::CreateInstance(const char* title, unsigned int width, unsigned int height)
{
    bool status = true;
    #define SDL_ASSERT(b, e) if(!(b)) { SDL_ERROR(e); status = false; }

	SDL_ASSERT(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "SDL initialization failure");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if(status)
    {
        g_Window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
        SDL_ASSERT(g_Window != NULL, "SDL window creation failure");
    }

    if(status)
    {
	    g_Context = SDL_GL_CreateContext(g_Window);
    	SDL_ASSERT(g_Context != NULL, "SDL OpenGL context creation failure");
    }

    if(status)
    {
    	g_Keyboard = SDL_GetKeyboardState(nullptr);
    }

    return status;
}

void Context::DeleteInstance()
{
	if (g_Context != nullptr) {
		SDL_GL_DeleteContext(g_Context);
	}

	if (g_Window != nullptr) {
		SDL_DestroyWindow(g_Window);
	}

	SDL_Quit();
}

void Context::Update()
{
	SDL_GL_SwapWindow(g_Window);
}

bool Context::PollEvent(Event& e)
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
			default: break;
		}
	}
	
	return false;
}

uint32_t Context::GetTime()
{
	return SDL_GetTicks();
}

bool Context::GetKeyState(KEY_CODE code)
{
	return (g_Keyboard[g_KeyMap[code]] != 0);
}
