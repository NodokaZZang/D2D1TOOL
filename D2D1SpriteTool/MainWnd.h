#pragma once
#include "pch.h"
class MainWnd : public D2D1Wnd
{

private:
	Text* m_mousePosText;
	Text* m_pixelPosText;
	DWORD m_keyColor;
	HWND  m_scroll;

public:
	MainWnd(HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd() {};
	virtual ~MainWnd() {};
public:
	void SetScroll(HWND _hwnd) { m_scroll = _hwnd; }
	void Vscroll(WPARAM wParam, LPARAM lParam);
	void MouseWheel(WPARAM wParam, LPARAM lParam);
	void MenuBind(int _menu, UINT& _nowMenu);
	void KeyDown(WPARAM _param);
	void ClearAnimation();
	void SaveItem(ResourceObj* _obj);
	void SaveGameObject(Sprite* _sprite);
	void CreateBitmap(ResourceObj* _obj);
private:
	void FileOpenProc();
	void KeyColorErase();
	void AutoSprite();
	void DragSprite();
	void Clear();
	void ClearAll();
	void ParseSprite();
	void MakeAnimation();
	void SelectSprite();
	void FileSaveProc();
	void SaveSprite();
	void SaveAnimation();
	void LoadSprite();
	void EraseBitmap();
	bool CanParse(Sprite* _spirte);
	bool CanSelect(Sprite* _sprite);
private:
	Pos  GetMousePosXY(int _x, int _y);
private:
	// JOBQUEUE
	std::queue<Pos>			 m_jobQueue;
	bool** m_discovered = nullptr;
	int						 m_prevBitmapWidth;
	int						 m_prevBitmapHeight;
	HWND					 m_mainScrollY;
	void					 BFS(Pos _pos);
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
	virtual void			 Resize(UINT _width, UINT _height) override;
	virtual void			 Render() override;
public:
	static BOOL CALLBACK	 FPSSettingBox(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam);
};

