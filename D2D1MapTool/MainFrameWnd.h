#pragma once
#include "pch.h"
class MainFrameWnd : public Wnd
{
public:
	MainFrameWnd(HINSTANCE _instance, const TCHAR* _className, const TCHAR* _title, DWORD _width, DWORD _height, int _ncmdShow);
	MainFrameWnd() {};
	virtual ~MainFrameWnd() {};
private:
	class MainWnd* m_mainWnd; // 화면
	class TreeViewWnd* m_treeViewWnd; // 메뉴

public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
};

