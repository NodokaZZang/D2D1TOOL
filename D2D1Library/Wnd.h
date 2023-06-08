#pragma once
/***********************************************
	Wnd: 윈도우 창 최상위 클래스			
***********************************************/
class Wnd
{
protected:
	HWND					m_hwnd;
	int						m_startMousePosX;
	int						m_startMousePosY;
	int						m_endMousePosX;
	int						m_endMousePosY;
	int						m_width;
	int						m_height;
	int						m_ncmdShow;
	int						m_x;
	int						m_y;
	const					TCHAR* m_className;
	const					TCHAR* m_titleName;
	HINSTANCE				m_instance;
	HWND					m_hParent;
	UINT					m_nowMenu;
	int						m_scrollY = 0;
	int						m_scrollX = 0;
public:
	Wnd() {};
	virtual ~Wnd() {};
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) abstract;
	virtual	void			 Init() abstract;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) abstract;
public:
	HWND					 GetHwnd() { return m_hwnd; }
	void					 SetHwnd(HWND _hwnd) { m_hwnd = _hwnd; }
	RECT					 GetClientSizeRect();
	TCHAR*					 FileOpen();
	TCHAR*					 FileSave();
public:
	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


};

