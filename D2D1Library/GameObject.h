#pragma once
class GameObject : public ResourceObj
{
private:
	char m_fileName[256];
	int  m_xPos;
	int  m_yPos;
	int  m_width;
	int  m_height;
	int  m_objectCode;
	D2D1_POINT_2F m_pivotPos;
public:
	GameObject(const ObjDataBinaryFile& _data);
	GameObject();
	int GetXPos() { return m_xPos; }
	int GetYPos() { return m_yPos; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	int GetObjCode() { return m_objectCode; }
	char* GetFileName() { return m_fileName; }
	D2D1_POINT_2F GetPivotPos() { return m_pivotPos; }

	void SetXPos(int _xPos) { m_xPos = _xPos; }
	void SetYPos(int _yPos) { m_yPos = _yPos; }
	void SetWidth(int _widht) { m_width = _widht; }
	void SetHeight(int _height) { m_height = _height; }
	void SetObjCode(int _objCode) { m_objectCode = _objCode; }
	void SetFileName(const char* _fileName) { strcpy_s(m_fileName, _fileName); }
	void SetPivotPos(const D2D1_POINT_2F& pivot) { m_pivotPos = pivot; }
private:
public:
};

