// include the basic windows header file

#include <windowsx.h>
#include "Window.h"
#include "CoreGraphics.h"
#include "Game.h"

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
			Game theGame( window );
			// Enter the infinite message loop
			while( window.readMessage( ) )
			{
				theGame.RunGame();
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
		const std::wstring eMsg = e.GetErrorMessage() + 
			L"\n\nException caught at Windows message loop.";
		MessageBox(nullptr, eMsg.c_str(),e.GetExceptionType().c_str(),MB_OK );
	}

	// return this part of the WM_QUIT message to Windows
	return 0;
}