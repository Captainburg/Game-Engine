#include "WindowBox.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	WindowBox wndBox(hInstance, iCmdShow); //Create Window

	return wndBox.RunLoop(); //Pass Control to Window
}