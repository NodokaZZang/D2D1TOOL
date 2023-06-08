#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const ObjDataBinaryFile& _data) : ResourceObj(GAME_OBJECT)
{
	SetFileName(_data.fileName);
	SetWidth(_data.width);
	SetHeight(_data.height);
	SetXPos(_data.xPos);
	SetYPos(_data.yPos);
	SetObjCode(_data.objectCode);
	SetPivotPos(_data.pivotPos);
}
