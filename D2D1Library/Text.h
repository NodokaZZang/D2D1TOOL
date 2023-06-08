#pragma once
#include "pch.h"

class Text
{
private:
	WCHAR			m_buffer[4096];
	D2D1_RECT_F		m_rect;
public:
	template<typename ... Types>
	Text(const WCHAR* _format, const D2D1_RECT_F& _rect, Types ...args);
	~Text() {};
public:
	template<typename ... Types>
	void Update(const WCHAR* _format, Types ...args);
	WCHAR* GetText() { return m_buffer; };
	D2D1_RECT_F	GetRect() { return m_rect; };
	void		SetRect(const D2D1_RECT_F& _rect) { m_rect = _rect; }
};

template<typename ...Types>
inline Text::Text(const WCHAR* _format, const D2D1_RECT_F& _rect, Types ...args)
{
	swprintf_s(m_buffer, _format, args...);
	m_rect = _rect;
}

template<typename ...Types>
inline void Text::Update(const WCHAR* _format, Types ...args)
{
	if (wcslen(m_buffer) == 0)
		return;
	swprintf_s(m_buffer, _format, args...);
}
