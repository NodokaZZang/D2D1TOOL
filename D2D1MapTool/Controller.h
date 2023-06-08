#pragma once
class MainFrameWnd;
class MainWnd;
class TreeViewWnd;
class Controller
{
private:
	static Controller* m_instance;
private:
	MainFrameWnd* m_mfWnd;
	MainWnd* m_mWnd;
	TreeViewWnd* m_treeViewWnd;
	int			 m_frame = 50;
public:
	void SetMainFrameWnd(MainFrameWnd* _wnd) { m_mfWnd = _wnd; }
	void SetMainWnd(MainWnd* _wnd) { m_mWnd = _wnd; }
	void SetTreViewWnd(TreeViewWnd* _wnd) { m_treeViewWnd = _wnd; }

	MainFrameWnd* GetMainFrameWnd() { return m_mfWnd; }
	MainWnd* GetMainWnd() { return m_mWnd; }
	TreeViewWnd* GetTreeViewWnd() { return m_treeViewWnd; }
public:
	void Vscroll(WPARAM wParam, LPARAM lParam);
	void MouseWheel(WPARAM wParam, LPARAM lParam);
	void Render();
	void MenuBind(int _menu, UINT& _nowMenu);
	void KeyDown(WPARAM param);

	int GetFrame() { return m_frame; }
	void SetFrame(int _frame) { m_frame = _frame; }
public:
	static Controller* GetInstance();
};

