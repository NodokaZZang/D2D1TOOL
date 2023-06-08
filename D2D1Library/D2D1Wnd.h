#pragma once
#include "pch.h"
/***********************************************
	D2D1Wnd: 윈도우 창 최상위 클래스
***********************************************/
class Text;
class Sprite;
class D2D1Wnd : public Wnd
{
protected:
	ID2D1HwndRenderTarget*			m_rt;
	ID2D1SolidColorBrush*			m_brush;
	ID2D1SolidColorBrush*			m_brush1;
	ID2D1SolidColorBrush*			m_brush2;
	ID2D1SolidColorBrush*			m_brush3;
	ID2D1SolidColorBrush*			m_brush4;
	ID2D1SolidColorBrush*			m_brush5;
	IDWriteTextFormat*				m_textFormat;
	Bitmap*							m_bitmap;
	std::vector<Text*>				m_textVector;
	std::vector<TargetSprite>		m_spriteVector;
	std::vector<TargetSprite>	    m_targetrectVector;
public:
	D2D1Wnd() : Wnd() {};
	virtual							~D2D1Wnd() {};
public:
	virtual	LRESULT CALLBACK		DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) abstract;
	virtual	void					Init() abstract;
	virtual void					Resize(UINT _width, UINT _height) abstract;
	virtual void					Render() abstract;
public:
	std::vector<TargetSprite>&		GetTargetVector() { return m_targetrectVector; }
	std::vector<TargetSprite>&		GetSpriteVector() { return m_spriteVector; }
	Bitmap*							GetBitmap() { return m_bitmap; }
	void							SetBrush(D2D1::ColorF _color);
	void							SetBrush(D2D1::ColorF _color, ID2D1SolidColorBrush** _brsuh);
	void							AddText(Text* _text);
	void							SetBitmap(Bitmap* _bitmap) { m_bitmap = _bitmap; };
	void							EraseBitmap();
	DWORD							GetPosBitmapMemory(int _x, int _y);
};
