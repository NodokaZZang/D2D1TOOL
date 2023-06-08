#include "pch.h"
#include "Controller.h"
#include "MainFrameWnd.h"
#include "MainWnd.h"
#include "TreeViewWnd.h"
#include "PreViewWnd.h"
Controller* Controller::m_instance = nullptr;

void Controller::Vscroll(WPARAM wParam, LPARAM lParam)
{
	if (m_mWnd)
		m_mWnd->Vscroll(wParam, lParam);
}

void Controller::MouseWheel(WPARAM wParam, LPARAM lParam)
{
	if (m_mWnd)
		m_mWnd->MouseWheel(wParam, lParam);
}

void Controller::Render()
{
	if (m_mWnd)
		m_mWnd->Render();

	if (m_treeViewWnd)
		m_treeViewWnd->Render();

	if (m_preViewWnd)
		m_preViewWnd->Render();
}

void Controller::MenuBind(int _menu, UINT& _nowMenu)
{
	if (m_mWnd)
		m_mWnd->MenuBind(_menu, _nowMenu);
}

void Controller::KeyDown(WPARAM param)
{
	if (m_mWnd)
		m_mWnd->KeyDown(param);
}


Controller* Controller::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new Controller();
	return m_instance;
}
