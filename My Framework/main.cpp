// include the basic windows header file

#include <windowsx.h>
#include "Window.h"
#include "CoreGraphics.h"

// the WindowProc function prototype

// the entry point for any Windows program
int WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE,
	LPWSTR lpCmdLine,
	INT)
{
	CoreWindow window( hInstance,lpCmdLine );
	CoreGraphics gfx( window );

	// Enter the infinite message loop
	while(window.readMessage())
	{

	}

	// return this part of the WM_QUIT message to Windows
	return 0;
}