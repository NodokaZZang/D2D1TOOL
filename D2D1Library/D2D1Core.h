#pragma once
/***********************************************
	D2D1Core: 그래픽 관련 인터페이스
***********************************************/
class Bitmap;
class D2D1Core
{
private:
	D2D1Core() = default;
	~D2D1Core() {};
private:
	ID2D1Factory*					g_pD2D1Factory;
	ID2D1HwndRenderTarget*			g_pD2D1RenderTarget;
	IDWriteFactory*					g_pD2D1DWriteFactory;
	IWICImagingFactory*				g_pWICFactory;
	ID2D1BitmapRenderTarget*		g_pCompatibleRenderTarget;
	static	D2D1Core*				m_instance;
public:
	void							Init();
	void							CreateRenderTarget(HWND _hwnd, ID2D1HwndRenderTarget** _rt);
	void							CreateRenderTarget(ID2D1HwndRenderTarget* _rt, ID2D1BitmapRenderTarget** _crt);
	static				  D2D1Core* GetInstance();
	Bitmap*							LoadBitmapByFileName(const PCWSTR _fileName);
	Bitmap*							LoadBitmapByFileName(ID2D1HwndRenderTarget** _rt, const PCWSTR _fileName);
	void							SaveFile(Bitmap* _bitmap, const PCWSTR _fileName);
	void							SetFontFormat(IDWriteTextFormat** _format, const WCHAR _fontName[], const FLOAT _fontSize);
	void*							ConvertFrameToBitmap(IWICBitmapFrameDecode* frame);
	IWICImagingFactory*				GetIWICImangFactory() { return g_pWICFactory; }
	void							CreateBitmap(const PCWSTR _fileName, Bitmap* _bitmap, Sprite* _obj);
	TCHAR*							FileOpen();


};

