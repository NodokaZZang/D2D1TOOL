#include "pch.h"
#include "MainFrameWnd.h"
#include "MainWnd.h"
#include "TreeViewWnd.h"
#include "resource.h"
#include "Controller.h"
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
	wecx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
	//CWnd::CreateWnd(m_className, m_titleName, m_width, m_height, m_instance);
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

	Controller::GetInstance()->SetMainFrameWnd(this);
	m_mainWnd = new MainWnd(m_instance,m_hwnd, L"MAIN", L"JGSPRITETOOL", _width - (mainWidth), 0, mainWidth - ScrollBarWidth, size.bottom - 50, 0);
	m_treeViewWnd = new TreeViewWnd(m_instance, m_hwnd, L"SUB", L"JGSPRITETOOL", 0, 0, _width - (mainWidth), size.bottom - 50, 0);

	HWND y_scroll = CreateWindowEx(
		0,
		L"SCROLLBAR",
		NULL,
		WS_VISIBLE | WS_CHILD | SBS_VERT,
		_width - (mainWidth) + mainWidth - ScrollBarWidth,
		0,
		ScrollBarWidth,
		size.bottom - 50,
		m_hwnd,
		(HMENU)NULL,
		m_instance,
		NULL);

	m_mainWnd->SetScroll(y_scroll);
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
		m_mainWnd->MenuBind(LOWORD(wParam));
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