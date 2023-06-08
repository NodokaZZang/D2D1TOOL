#pragma once
class SubFrameWnd :public Wnd
{
public:
								 SubFrameWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
								 ~SubFrameWnd() {};
private:
	class  PreViewWnd*			 m_preViewWnd;
	class  TreeViewWnd*			 m_treeViewWnd;
public:
	void						 Render();
public:
	virtual	LRESULT CALLBACK	 DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void				 Init() override;
	virtual void				 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
};

