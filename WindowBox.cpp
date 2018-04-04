#include "WindowBox.h"
#include "GameMacros.h"

WindowBox::WindowBox(HINSTANCE hInstance, int iCmdShow)
	: game()
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("GameWindow");
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);
	hwnd = CreateWindow(
		TEXT("GameWindow"),
		GAME_NAME,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		RESOLUTION_X, RESOLUTION_Y,
		NULL, NULL,
		hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	game.GameInit(hwnd);
}

WindowBox::~WindowBox()
{
}

WPARAM WindowBox::RunLoop()
{
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			if (msg.message == WM_LBUTTONDOWN)
			{
				game.GetRay(LOWORD(msg.lParam), HIWORD(msg.lParam));
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game.GameLoop();
		}
	}
	game.GameShutdown();
	return msg.wParam;
}

LRESULT CALLBACK WindowBox::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}
