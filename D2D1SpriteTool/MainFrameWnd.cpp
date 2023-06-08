#include "pch.h"
#include "MainFrameWnd.h"
#include "MainWnd.h"
#include "resource.h"
#include "Controller.h"
#include "SubFrameWnd.h"
MainFrameWnd::MainFrameWnd(HINSTANCE _instance, const TCHAR _className[], const TCHAR _title[], DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_className = _className;
	m_titleName = _title;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	Init();
}

void MainFrameWnd::Init()
{
	WNDCLASSEX wecx;
	wecx.cbSize = sizeof(WNDCLASSEX);
	wecx.style = CS_HREDRAW | CS_VREDRAW;
	wecx.cbClsExtra = 0;
	wecx.cbWndExtra = sizeof(LONG_PTR);
	wecx.hInstance = m_instance;
	wecx.hIcon = LoadIcon(m_instance, IDI_APPLICATION);
	wecx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wecx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wecx.lpszClassName = m_className;
	wecx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wecx.hIconSm = LoadIcon(wecx.hInstance, IDI_APPLICATION);
	wecx.lpfnWndProc = Wnd::WndProc;  // 정적함수
	if (!RegisterClassEx(&wecx))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx Main failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return;
	}
	CreateWnd(m_className, m_titleName, m_width, m_height, m_instance);
}

void MainFrameWnd::CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance)
{
	m_hwnd = CreateWindow(
		_className,
		_titleName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		_width, _height,
		NULL,
		NULL,
		_instance,
		this);

	RECT size = GetClientSizeRect();
	int mainWidth = _width / 7 * 6;
	int ScrollBarWidth = 15;
	m_mainWnd = new MainWnd(m_hwnd, L"MAIN", L"JGSPRITETOOL", 0, 0, mainWidth, size.bottom - 50, 0);
	Controller::GetInstance()->SetMainFrameWnd(this);
	m_subFrameWnd = new SubFrameWnd(_instance, m_hwnd, L"SUB", L"JGSPRITETOOL", mainWidth + ScrollBarWidth, 0, _width - (mainWidth + ScrollBarWidth), size.bottom - 50, 0);
	HWND scroll = CreateWindowEx(
		0,
		L"SCROLLBAR",
		NULL,
		WS_VISIBLE | WS_CHILD | SBS_VERT,
		mainWidth,
		0,
		ScrollBarWidth,
		size.bottom - 50,
		m_hwnd,
		(HMENU)NULL,
		m_instance,
		NULL);
	m_mainWnd->SetScroll(scroll);
	ShowWindow(m_hwnd, SW_MAXIMIZE);
	UpdateWindow(m_hwnd);
}

LRESULT MainFrameWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HMENU hMenu;
	switch (message)
	{
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		CheckMenuRadioItem(hMenu, ID_TOOL_DRAGSPRITE, ID_TOOL_DRAGSPRITE, NULL, MF_BYCOMMAND);
		break;

	case WM_KEYUP:
		break;

	case WM_KEYDOWN:
		Controller::GetInstance()->KeyDown(wParam);
		break;

	case WM_TIMER:
		break;

	case WM_SIZE:
		break;

	case WM_COMMAND:
		Controller::GetInstance()->MenuBind(LOWORD(wParam), m_nowMenu);
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONUP:
		break;

	case WM_MOUSEWHEEL:
		break;

	case WM_VSCROLL:
		Controller::GetInstance()->Vscroll(wParam, lParam);
		break;

	case WM_PAINT:
		Controller::GetInstance()->Render();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}