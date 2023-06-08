#include "pch.h"
#include "Wnd.h"
#include <CommDlg.h> 

RECT Wnd::GetClientSizeRect()
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	return rc;
}

LRESULT Wnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Wnd* pWnd = nullptr;
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT pCS = (LPCREATESTRUCT)lParam;
		SetLastError(0);
		pWnd = reinterpret_cast<Wnd*>(pCS->lpCreateParams);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)))
		{
			if (GetLastError() != 0) return E_FAIL;
		}
	}
	else
	{
		pWnd = reinterpret_cast<Wnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (message == WM_GETMINMAXINFO && pWnd == nullptr)
		return S_OK;

	return pWnd->DisPatch(hWnd, message, wParam, lParam);
}

TCHAR* Wnd::FileOpen()
{
	OPENFILENAME	OFN;
	TCHAR filter[] = _T("Every File(*.*) \0*.*\0Img File\0*.png;*.jpg\0\0");
	TCHAR lpstrFile[500] = L"";
	TCHAR filePathName[500] = L"";
	::memset(&OFN, 0, sizeof(OFN));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hwnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0)
	{
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		return OFN.lpstrFile;
	}
	else
	{
		return nullptr;
	}
}

TCHAR* Wnd::FileSave()
{
	OPENFILENAME	OFN;
	TCHAR filter[] = _T("Every File(*.*) \0*.*\0Img File\0*.png;*.jpg\0\0");
	TCHAR lpstrFile[500] = L"";
	TCHAR filePathName[500] = L"";
	::memset(&OFN, 0, sizeof(OFN));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hwnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	OFN.lpstrInitialDir = L".";

	if (GetSaveFileName(&OFN) != 0)
	{
		wsprintf(filePathName, L"%s 파일을 저장하시겠습니까?", OFN.lpstrFile);
		return OFN.lpstrFile;
	}
	else
	{
		return nullptr;
	}
}
