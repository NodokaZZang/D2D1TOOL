#include "pch.h"
#include "D2D1Wnd.h"
#include "Bitmap.h"
void D2D1Wnd::SetBrush(D2D1::ColorF _color)
{
	m_rt->CreateSolidColorBrush(
		_color,
		&m_brush
	);
}

void D2D1Wnd::SetBrush(D2D1::ColorF _color, ID2D1SolidColorBrush** _brsuh)
{
	m_rt->CreateSolidColorBrush(
		_color,
		_brsuh
	);
}

void D2D1Wnd::AddText(Text* _text)
{
	m_textVector.push_back(_text);
}

void D2D1Wnd::EraseBitmap()
{
	m_bitmap = nullptr;
	delete m_bitmap;
	m_targetrectVector.clear();
	m_spriteVector.clear();
}

DWORD D2D1Wnd::GetPosBitmapMemory(int _x, int _y)
{
	if (m_bitmap == nullptr)
		return -1;
	const int widht = m_bitmap->GetWidht();
	const int height = m_bitmap->GetHeight();
	if (_x < 0 || _y < 0 || _x >= widht || _y >= height)
		return -1;
	return ((DWORD*)m_bitmap->GetMemory())[_x + (widht * _y)];;
}
