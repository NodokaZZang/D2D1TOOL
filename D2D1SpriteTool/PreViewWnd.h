#pragma once
class PreViewWnd : public D2D1Wnd
{
private:

public:
	PreViewWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
	~PreViewWnd() {};
private:
	ResourceObj* m_target;
	Bitmap* m_bitmap;
private:
	int		m_lastTick = 0;
	int		m_sumTick = 0;
public:
	void	SetSprite(ResourceObj* _sprite, Bitmap* _bitmap);
	void	RenderSprite(ResourceObj* _sprite);
	void	RenderSpriteProc(Sprite* _sprite);
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
	virtual void			 Resize(UINT _width, UINT _height) override;
	virtual void			 Render() override;
};

