#pragma once
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <string>
#include "CoreGraphics.h"
#include "Keyboard.h"

static bool rightIsPressed = false;

class WindowKey
{
	friend CoreGraphics::CoreGraphics( WindowKey&,Keyboard& );
public:
	WindowKey( const WindowKey& ) = delete;
	WindowKey& operator=( WindowKey& ) = delete;
protected:
	WindowKey() = default;
protected:
	HWND window = nullptr;
};

class CoreWindow : public WindowKey
{
public:
	CoreWindow( HINSTANCE hInst,wchar_t* lpCmdLine);
	void DisplayMessage( const std::wstring& title,const std::wstring& message ) const;

	bool readMessage();
private:
	static LRESULT WINAPI WindowProcInitial(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static LRESULT WINAPI CoreWindow::WindowProcFinal( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );

	LRESULT CoreWindow::HandleMsg( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Keyboard keyboard;
private:
	static constexpr wchar_t* wndClassName = L"My Framework";
	HINSTANCE hInst = nullptr;
	std::wstring lpCmdLine;
};