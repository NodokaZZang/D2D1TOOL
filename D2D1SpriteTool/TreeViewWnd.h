#pragma once
#include <commctrl.h>
class TreeViewWnd : public Wnd
{
public:
	TreeViewWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
	~TreeViewWnd() {};
public:
	HTREEITEM 	AddItemToTree(int TVID, LPTSTR _lpszItem, HTREEITEM hParent, HTREEITEM hInsAfter, LPARAM data);
	void		Render();
	HTREEITEM	GetSpriteRoot() { return m_spriteRoot; }
	HTREEITEM	GetAnimationRoot() { return m_animationRoot; }
	HWND		GetTreeViewHwnd() { return m_treeviewHwnd; }
	HTREEITEM   GetCusorSel();

	static int			GetSpriteSq()
	{
		return ++spriteSq;
	}

	static int			GetAnimationSq()
	{
		return ++animationSq;
	}

private:
	static int spriteSq;
	static int animationSq;

private:
	HWND	m_treeviewHwnd;
	HTREEITEM m_spriteRoot;
	HTREEITEM m_animationRoot;
	int		g_nOpen;
	int		g_nClosed;
	int		g_nDocument;
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
};

