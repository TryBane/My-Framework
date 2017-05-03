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
	try
	{
		CoreWindow window( hInstance,lpCmdLine );

		try
		{
			CoreGraphics gfx( window );

			// Enter the infinite message loop
			while( window.readMessage( ) )
			{

			}
		}
		catch( const MyException& e )
		{
			const std::wstring eMsg = e.GetErrorMessage() + 
				L"\n\nException caught at Windows message loop.";
			window.DisplayMessage( e.GetExceptionType(),eMsg );
		}
	}
	catch( const MyException& e )
	{
		
	}
	// return this part of the WM_QUIT message to Windows
	return 0;
}