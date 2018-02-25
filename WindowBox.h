#include <windows.h>
#include "Game.h"

class WindowBox {
public:
	WindowBox(HINSTANCE hInstance, int iCmdShow);
	virtual ~WindowBox();

	/** Run Loop
	*
	* Constantly executes and tells game to continue, while it handles windows messages.
	*
	* @return WPARAM The wParam is returned to main for error handling.
	*/
	WPARAM RunLoop();

private:
	/** Window Proc
	*
	* Handles all windows related callbacks.
	*
	* @param hwnd The Handle to the window.
	* @param message The windows message that needs to be resolved.
	* @param wparam Additional message information.
	* @param lparam Additional message information.
	* @return LRESULT A message dependent return message indicating that the callback has been handled.
	*/
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	HWND hwnd;
	Game game;
};