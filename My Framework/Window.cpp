#include "Window.h"

CoreWindow::CoreWindow( HINSTANCE hInst,wchar_t* lpCmdLine)
	:
	lpCmdLine( lpCmdLine ),
	hInst( hInst )
{
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ),CS_CLASSDC,WindowProcInitial,0,0,
		hInst,nullptr,nullptr,nullptr,nullptr,
		wndClassName,nullptr };
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClassEx( &wc );

	// calculate the size of the client area
	RECT wr = {0, 0, CoreGraphics::ScreenWidth, CoreGraphics::ScreenHeight};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	window = CreateWindowEx(NULL,
		wndClassName,    // name of the window class
		L"My Framework",    // title of the window
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,    // window style
		CoreGraphics::ScreenOriginx,    // x-position of the window
		CoreGraphics::ScreenOriginy,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		nullptr,    // we have no parent window, NULL
		nullptr,    // we aren't using menus, NULL
		hInst,    // application handle
		this);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(window,SW_SHOWDEFAULT);
}

bool CoreWindow::readMessage()
{
	MSG msg = {0};
	while( PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )
	{
		// translate keystroke messages into the right format
		TranslateMessage( &msg );

		// send the message to the WindowProc function
		DispatchMessage( &msg );

		// check to see if it's time to quit
		if( msg.message == WM_QUIT )
		{
			return false;
		}

	}
	return true;
}

// this is the main message handler for the program
LRESULT WINAPI CoreWindow::WindowProcInitial( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if( msg == WM_NCCREATE )
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		CoreWindow* const pWnd = reinterpret_cast<CoreWindow*>( pCreate->lpCreateParams );
		// sanity check
		assert( pWnd != nullptr );
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>( pWnd ) );
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>( &CoreWindow::WindowProcFinal ) );
		// forward message to window class handler
		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd,msg,wParam,lParam );
}

LRESULT WINAPI CoreWindow::WindowProcFinal( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	// retrieve ptr to window class
	CoreWindow* const pWnd = reinterpret_cast<CoreWindow*>( GetWindowLongPtr( hWnd,GWLP_USERDATA ) );
	// forward message to window class handler
	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
}

LRESULT CoreWindow::HandleMsg( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch(message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc (hWnd, message, wParam, lParam);
}