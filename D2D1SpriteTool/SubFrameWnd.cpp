#include "pch.h"
#include "SubFrameWnd.h"
#include "Controller.h"
#include "TreeViewWnd.h"
#include "PreViewWnd.h"
SubFrameWnd::SubFrameWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_hParent = _parent;
	m_className = _className;
	m_titleName = _title;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	m_x = _x;
	m_y = _y;
	Init();
}

void SubFrameWnd::Render()
{
	
}

void SubFrameWnd::Init()
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
	wecx.lpszMenuName = nullptr;
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


void SubFrameWnd::CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance)
{
	m_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_className,
		NULL,
		WS_CHILD | WS_VISIBLE,
		m_x, m_y,
		_width, _height,
		m_hParent,
		NULL,
		GetModuleHandle(NULL),
		this);

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	int mainHeight = (_height / 5) * 3;
	int padding = 15;
	Controller::GetInstance()->SetSubFrameWnd(this);
	m_treeViewWnd = new TreeViewWnd(m_instance, m_hwnd, L"TREE", L"JGSPRITETOOL", 0, 0, rc.right, mainHeight, 0);
	m_preViewWnd = new PreViewWnd(m_instance, m_hwnd, L"PRE", L"JGSPRITETOOL", 0, mainHeight + padding, rc.right, _height - (padding + mainHeight), 0);

	Controller::GetInstance()->SetTreViewWnd(m_treeViewWnd);
	Controller::GetInstance()->SetPreViewWnd(m_preViewWnd);
}

LRESULT SubFrameWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_KEYUP:
		break;

	case WM_KEYDOWN:
		break;

	case WM_TIMER:
		break;

	case WM_SIZE:
		break;

	case WM_COMMAND:
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONUP:
		break;

	case WM_PAINT:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

