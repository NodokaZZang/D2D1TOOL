#include "pch.h"
#include "App.h"
#include "MainFrameWnd.h"
void App::Init(HINSTANCE _hInstance, const TCHAR _title[], const TCHAR _className[], DWORD _width, DWORD _height, int _ncmdShow)
{
	D2D1Core::GetInstance()->Init();
	m_MainWindow = new MainFrameWnd(_hInstance, _T("DOM"), _title, _width, _height, _ncmdShow);
}

int App::Dispatch()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
