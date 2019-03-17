#include <windows.h>

#include <gk/context.hpp>

#include <stdio.h>

#include <string>

#include <gk/assert.hpp>
#include <gk/graphics.hpp>

struct CTX_PRIVATE_DATA
{
	HMODULE hProcess;
	HWND	hWindow;
	HDC		hDeviceCtx;
	HGLRC	hRenderCtx;

	unsigned int width;
	unsigned int height;

	bool is_running;

	byte key_map[Context::KEYBOARD_LENGTH]; // Windows key codes: https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
	byte keyboard[Context::KEYBOARD_LENGTH];
};

Context* Instance = nullptr;

const char GK_WIN_CLS[] = "GK_GL_CTX";
const wchar_t W_GK_WIN_CLS[] = L"GK_GL_CTX";

// callback function to process events
LRESULT WinProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitializeKeyboard(byte* key_map)
{
	memset(key_map, 0, Context::KEYBOARD_LENGTH);

	key_map['Q']       = Context::KEY_Q;
	key_map['W']       = Context::KEY_W;
	key_map['E']       = Context::KEY_E;
	key_map['A']       = Context::KEY_A;
	key_map['S']       = Context::KEY_S;
	key_map['D']	   = Context::KEY_D;
	key_map[VK_UP]	   = Context::KEY_UP;
	key_map[VK_DOWN]   = Context::KEY_DOWN;
	key_map[VK_LEFT]   = Context::KEY_LEFT;
	key_map[VK_RIGHT]  = Context::KEY_RIGHT;
	key_map[VK_SPACE]  = Context::KEY_SPACE;
	key_map[VK_ESCAPE] = Context::KEY_ESCAPE;
}

bool Context::CreateInstance(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char* title)
{
	if (Instance == nullptr)
	{
		Instance = new Context();
		
		RenderingContext* rCtx = nullptr;

		CTX_PRIVATE_DATA* pData = new CTX_PRIVATE_DATA();
		Instance->DATA = pData;

		pData->hProcess = GetModuleHandle(nullptr);
		pData->hWindow = nullptr;
		pData->hDeviceCtx = nullptr;
		pData->hRenderCtx = nullptr;

		//wchar_t _title[64] = { 0 };									// wide char title buffer
		unsigned int pixel_format = 0;								// pixel format
		unsigned int win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN; // window style
		unsigned int ext_style = 0;									// window extended style

		WNDCLASS wClass;
		PIXELFORMATDESCRIPTOR descriptor = { 0 };

		wClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wClass.lpfnWndProc = (WNDPROC)WinProc;
		wClass.cbClsExtra = 0;
		wClass.cbWndExtra = 0;
		wClass.hInstance = pData->hProcess;
		wClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wClass.hbrBackground = nullptr;
		wClass.lpszMenuName = nullptr;
		wClass.lpszClassName = GK_WIN_CLS;

		if (!RegisterClass(&wClass))
		{
			printf("Error: Failed to register class\n");
			goto FAILURE;
		}

		//swprintf(_title, 63, L"%s", title);

		pData->hWindow = CreateWindowEx(ext_style, GK_WIN_CLS, title,
			win_style, x, y, width, height,
			nullptr, nullptr, pData->hProcess, nullptr
		);

		if (pData->hWindow == nullptr)
		{
			printf("Error: Failed to create window\n");
			goto FAILURE;
		}

		pData->hDeviceCtx = GetDC(pData->hWindow);
		if (pData->hDeviceCtx == nullptr)
		{
			printf("Error: Failed to get create Win32 drawing context\n");
			goto FAILURE;
		}

		descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		descriptor.nVersion = 1;
		descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		descriptor.iPixelType = PFD_TYPE_RGBA;
		descriptor.cColorBits = 32;
		descriptor.cRedBits = 0;
		descriptor.cRedShift = 0;
		descriptor.cGreenBits = 0;
		descriptor.cGreenShift = 0;
		descriptor.cBlueBits = 0;
		descriptor.cBlueShift = 0;
		descriptor.cAlphaBits = 0;
		descriptor.cAlphaShift = 0;
		descriptor.cAccumBits = 0;
		descriptor.cAccumRedBits = 0;
		descriptor.cAccumGreenBits = 0;
		descriptor.cAccumBlueBits = 0;
		descriptor.cAccumAlphaBits = 0;
		descriptor.cDepthBits = 24;
		descriptor.cStencilBits = 8;
		descriptor.cAuxBuffers = 0;
		descriptor.iLayerType = PFD_MAIN_PLANE;
		descriptor.bReserved = 0;
		descriptor.dwLayerMask = 0;
		descriptor.dwVisibleMask = 0;
		descriptor.dwDamageMask = 0;

		pixel_format = ChoosePixelFormat(pData->hDeviceCtx, &descriptor);
		if (pixel_format == 0)
		{
			printf("Error: Could not find appropriate pixel format\n");
			goto FAILURE;
		}

		if (!SetPixelFormat(pData->hDeviceCtx, pixel_format, &descriptor))
		{
			printf("Error: Could not set the pixel format\n");
			goto FAILURE;
		}

		pData->hRenderCtx = wglCreateContext(pData->hDeviceCtx);
		if (pData->hRenderCtx == nullptr)
		{
			printf("Error: Could not create rendering context\n");
			goto FAILURE;
		}

		if (wglMakeCurrent(pData->hDeviceCtx, pData->hRenderCtx) != TRUE)
		{
			printf("Error: Could not bind render context\n");
			goto FAILURE;
		}

		if (!RenderingContext::CreateInstance()) {
			goto FAILURE;
		}
		rCtx = RenderingContext::GetInstance();

		InitializeKeyboard(pData->key_map);

		ShowWindow(pData->hWindow, SW_SHOW);
		SetForegroundWindow(pData->hWindow);
		SetFocus(pData->hWindow);

		pData->is_running = true;
		rCtx->SetViewport(0, 0, pData->width, pData->height);

		return true;

		FAILURE:
		{
			RenderingContext::DeleteInstance();
			DeleteInstance();
			return false;
		}
	}
	else
	{
		printf("Error: Context already exists\n");
		return false;
	}
}

bool Context::DeleteInstance()
{
	if (Instance != nullptr)
	{
		CTX_PRIVATE_DATA* pData = reinterpret_cast<CTX_PRIVATE_DATA*>(Instance->DATA);

		if (pData->hRenderCtx != nullptr)
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(pData->hRenderCtx);
		}

		if (pData->hDeviceCtx != nullptr)
		{
			ReleaseDC(pData->hWindow, pData->hDeviceCtx);
		}

		if (pData->hWindow != nullptr)
		{
			DestroyWindow(pData->hWindow);
		}

		UnregisterClass(GK_WIN_CLS, pData->hProcess);

		delete Instance->DATA;
		delete Instance;
		Instance = nullptr;

		return true;
	}
	else
	{
		printf("Error: Context is not allocated\n");
		return false;
	}
}

Context* Context::GetInstance()
{
	GK_ASSERT(Instance != nullptr, ("Error: Context does not exist\n"));
	return Instance;
}

void* Context::GetPrivateData()
{
	GK_ASSERT(Instance != nullptr, ("Error: Context does not exist\n"));
	return Instance->DATA;
}

byte* Context::GetKeyboard()
{
	GK_ASSERT(Instance != nullptr, ("Error: Context does not exist\n"));
	return reinterpret_cast<CTX_PRIVATE_DATA*>(Instance->DATA)->keyboard;
}

bool Context::Running()
{
	GK_ASSERT(Instance != nullptr, ("Error: Context does not exist\n"));
	return reinterpret_cast<CTX_PRIVATE_DATA*>(Instance->DATA)->is_running;
}

LRESULT WinProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CTX_PRIVATE_DATA* pData = reinterpret_cast<CTX_PRIVATE_DATA*>(Instance->GetPrivateData());

	switch (uMsg)
	{
		case WM_SIZE:
		{
			pData->width  = LOWORD(lParam);
			pData->height = HIWORD(lParam);
			break;
		}
		default:
		{
			return DefWindowProc(hWindow, uMsg, wParam, lParam);
		}
	};

	return 0;
}

void Context::ProcessEvents()
{
	GK_ASSERT(Instance != nullptr, ("Error: Context does not exist\n"));

	MSG msg;
	CTX_PRIVATE_DATA* pData = reinterpret_cast<CTX_PRIVATE_DATA*>(Instance->GetPrivateData());
	RenderingContext* rCtx = RenderingContext::GetInstance();

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
			case WM_SIZE:
			{
				pData->width  = LOWORD(msg.lParam);
				pData->height = HIWORD(msg.lParam);
				rCtx->SetViewport(0, 0, pData->width, pData->height);
			}
			case WM_QUIT:
			case WM_CLOSE:
			{
				pData->is_running = false;
				break;
			}
			case WM_KEYDOWN:
			{
				printf("Key down: %u => %hu\n", (unsigned int) msg.wParam, pData->key_map[msg.wParam]);
				pData->keyboard[pData->key_map[msg.wParam]] = true;
				break;
			}
			case WM_KEYUP:
			{
				printf("Key release: %u => %hu\n", (unsigned int)msg.wParam, pData->key_map[msg.wParam]);

				pData->keyboard[pData->key_map[msg.wParam]] = false;
				break;
			}
			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return;
}

void Context::Update()
{
	GK_ASSERT(Instance != nullptr, ("Error: Context does not exist\n"));

	CTX_PRIVATE_DATA* pData = reinterpret_cast<CTX_PRIVATE_DATA*>(Instance->DATA);
	SwapBuffers(pData->hDeviceCtx);
}
