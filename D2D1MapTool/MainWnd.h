#pragma once
#include "pch.h"
class Bitmap;
class MainWnd : public D2D1Wnd
{
public:
	MainWnd(HINSTANCE _instance, const TCHAR _className[], const TCHAR _title[], DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd() {};
	void MenuBind(int _menu);
	virtual ~MainWnd() {};
private:
	bool m_gridState = false;
	UINT m_gridXSize;
	UINT m_gridYSize;
	UINT m_gridWidth;
	int** m_mapData;
	std::map<char*, Bitmap*> m_bitmapDB;
	ID2D1BitmapRenderTarget* m_brt;
	HWND  m_scroll;
	FLOAT m_magnification = 1.0f;
	Bitmap* m_gameObjBitmap;


	class GameObject* m_target;
	class GameObject* m_player;
	class GameObject* m_nefendes;
	class GameObject* m_ghost;
	class GameObject* m_kuma;


private:
	void GridRender();
	void GameObjRender();
	void GameObjGridRender(const D2D1_RECT_F& rect, enum EVENT_TYPE _type);
	void ResourceLoad();	
	void FileOpenProc();
	void SaveFile();
	void MapBinaryFileSave(TCHAR* _path);
	void EreaseRender();
	void ClickEvent(int _x, int _y);
	void EventHandler(class Event* _obj, const Pos& _pos);
	void ObjHandler(class GameObject* _obj, const Pos& _pos);
	void ObjHandler(EVENT_TYPE _type, const Pos& _pos);
	Pos  GetMousePosXY(int _x, int _y);
	Pos  GetMousePosXYMap(int _x, int _y);
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
	virtual void			 Resize(UINT _width, UINT _height) override;
	virtual void			 Render() override;
public:
	static BOOL				 NewMapDiallog(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam);
	void					 GridSet(int _xSize, int _ySzie, int _width);
	void					 GridUnSet();
	void					 TreeViewClickEventBind(ResourceObj* _obj);
	void					 SetScroll(HWND _hwnd) { m_scroll = _hwnd; }
	void					 Vscroll(WPARAM wParam, LPARAM lParam);
	void					 MouseWheel(WPARAM wParam, LPARAM lParam);
	void					 KeyDown(WPARAM _param);
	Bitmap*					 BitmapGetInsert(char* _fileName);
	void					 SetPlayerObject(GameObject* _obj) { m_player = _obj; }
	void					 SetNefendesObject(GameObject* _obj) { m_nefendes = _obj; }
	void					 SetGhostObject(GameObject* _obj) { m_ghost = _obj; }
	void					 SetKumaObject(GameObject* _obj) { m_kuma = _obj; }
	void					 Insert(char* _fileName);
}
;