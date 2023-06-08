#pragma once
#include "ResourceObj.h"
class Sprite : public ResourceObj
{
private:
	D2D1_RECT_F		m_rect;
	D2D1_POINT_2F	m_pivotPos;
public:
	Sprite(const D2D1_RECT_F& _rect, const D2D1_POINT_2F& _pivotPos) : ResourceObj(SPRITE), m_rect(_rect), m_pivotPos(_pivotPos) {};
	Sprite() : ResourceObj(SPRITE) {};
	virtual ~Sprite() {};

	D2D1_RECT_F		GetRect() { return m_rect; }
	D2D1_POINT_2F	GetPivot() { return m_pivotPos; }
};

