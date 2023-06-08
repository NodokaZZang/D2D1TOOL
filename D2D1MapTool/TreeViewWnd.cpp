#include "pch.h"
#include "TreeViewWnd.h"
#include "pch.h"
#include "TreeViewWnd.h"
#include "MainWnd.h"
#include "Controller.h"
#include "Map.h"
#include "Event.h"
#include "GameObject.h"
#pragma comment(lib,"Comctl32.lib")

TreeViewWnd::TreeViewWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_hParent = _parent;
	m_className = _className;
	m_titleName = _title;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	m_x = _x;
	m_y = _y;
	Init();
}

void TreeViewWnd::Init()
{
	WNDCLASSEX wecx;
	wecx.cbSize = sizeof(WNDCLASSEX);
	wecx.style = CS_HREDRAW | CS_VREDRAW;
	wecx.cbClsExtra = 0;
	wecx.cbWndExtra = sizeof(LONG_PTR);
	wecx.hInstance = m_instance;
	wecx.hIcon = LoadIcon(m_instance, IDI_APPLICATION);
	wecx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wecx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wecx.lpszClassName = m_className;
	wecx.lpszMenuName = nullptr;
	wecx.hIconSm = LoadIcon(wecx.hInstance, IDI_APPLICATION);
	wecx.lpfnWndProc = Wnd::WndProc;  // 정적함수
	if (!RegisterClassEx(&wecx))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx Main failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return;
	}

	CreateWnd(m_className, m_titleName, m_width, m_height, m_instance);
}

void TreeViewWnd::CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance)
{
	m_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_className,
		NULL,
		WS_CHILD | WS_VISIBLE,
		m_x, m_y,
		_width, _height,
		m_hParent,
		NULL,
		GetModuleHandle(NULL),
		this);

	RECT rcClient;  // dimensions of client area 
	// Ensure that the common control DLL is loaded. 
	InitCommonControls();

	// Get the dimensions of the parent window's client area, and create 
	// the tree-view control. 
	GetClientRect(m_hwnd, &rcClient);
	m_treeviewHwnd = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | TVS_HASLINES
		| TVS_FULLROWSELECT | TVS_HASBUTTONS
		| TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_INFOTIP,
		m_x,
		m_y,
		_width,
		_height,
		m_hwnd,
		nullptr,
		m_instance,
		NULL);
	Controller::GetInstance()->SetTreViewWnd(this);
	m_mapRoot = AddItemToTree(0, (LPTSTR)L"Map", NULL, TVI_LAST, NULL);
	m_objectRoot = AddItemToTree(0, (LPTSTR)L"Object", NULL, TVI_LAST, NULL);
	m_scriptRoot = AddItemToTree(0, (LPTSTR)L"Script", NULL, TVI_LAST, NULL);

	Event* emptyEvent = new Event(EMPTY);
	Event* wallEvent = new Event(WALL);
	Event* NefendesEvent = new Event(NefendesRect);
	Event* GhostEvent = new Event(GhostRect);
	Event* KumaEvent = new Event(KumaRect);

	//	EMPTY = 0,
	//	WALL = 1,
	//	Player = 2,
	//	Nefendes = 3,
	//	Ghost = 4,
	//	Kuma = 5,
	//	NefendesRect = 6,
	//	GhostRect = 7


	AddItemToTree(0, (LPTSTR)L"EMPTY_EVENT", m_scriptRoot, TVI_LAST, (LPARAM)emptyEvent);
	AddItemToTree(0, (LPTSTR)L"WALL EVENT", m_scriptRoot, TVI_LAST, (LPARAM)wallEvent);
	AddItemToTree(0, (LPTSTR)L"NEFENDES EVENT", m_scriptRoot, TVI_LAST, (LPARAM)NefendesEvent);
	AddItemToTree(0, (LPTSTR)L"GHOST EVENT", m_scriptRoot, TVI_LAST, (LPARAM)GhostEvent);
	AddItemToTree(0, (LPTSTR)L"KUMA EVENT", m_scriptRoot, TVI_LAST, (LPARAM)KumaEvent);

	std::vector<TCHAR*> spriteFiles;
	
	GetFileList(spriteFiles, _T("*"));
	TCHAR exp [256];
	for (auto& fileName : spriteFiles) 
	{ 
		GetFileExp(fileName, exp);
		if (0 == _tcscmp(exp, _T("map"))) 
		{
			LoadMapData(fileName);
		}
		else if (0 == _tcscmp(exp, _T("obj")))
		{
			// AddItemToTree(0, (LPTSTR)fileName, m_objectRoot, TVI_LAST, NULL);
			LoadGameObjectData(fileName);
		}
	}
	for (int i = 0; i < spriteFiles.size(); i++)
		if (spriteFiles[i])
			delete spriteFiles[i];
}


HTREEITEM TreeViewWnd::AddItemToTree(int TVID, LPTSTR _lpszItem, HTREEITEM hParent, HTREEITEM hInsAfter, LPARAM data)
{
	TV_INSERTSTRUCT TVIS;
	TVIS.hParent = hParent;
	TVIS.hInsertAfter = hInsAfter;
	TVIS.item.mask = TVIF_TEXT | TVIF_PARAM;
	TVIS.item.pszText = _lpszItem;
	TVIS.item.lParam = data;

	return (HTREEITEM)SendMessage(m_treeviewHwnd, TVM_INSERTITEM, 0, (LPARAM)&TVIS);
}

void TreeViewWnd::Render()
{
	UpdateWindow(m_hwnd);
}

HTREEITEM TreeViewWnd::GetCusorSel()
{
	return (HTREEITEM)SendMessage(m_treeviewHwnd, TVM_GETNEXTITEM, TVGN_CARET, NULL);
}

void TreeViewWnd::LoadMapData(TCHAR* _fileName)
{
	FILE* p_file = NULL;
	char path[256] = "";
	WideCharToMultiByte(CP_ACP, 0, _fileName, 256, path, 256, NULL, NULL);
	fopen_s(&p_file, path, "rb");
	if (p_file != NULL)
	{
		MapDataBinaryFile* file = new MapDataBinaryFile;
		fread(file, sizeof(MapDataBinaryFile), 1, p_file);
		if (file)
		{
			Map* map = new Map(*file);
			AddItemToTree(0, (LPTSTR)_fileName, m_mapRoot, TVI_LAST, (LPARAM)map);
		}
		delete file;
	}
}

void TreeViewWnd::LoadGameObjectData(TCHAR* _fileName)
{
	FILE* p_file = NULL;
	char path[256] = "";
	WideCharToMultiByte(CP_ACP, 0, _fileName, 256, path, 256, NULL, NULL);
	fopen_s(&p_file, path, "rb");
	if (p_file != NULL)
	{
		ObjDataBinaryFile* file = new ObjDataBinaryFile;
		fread(file, sizeof(ObjDataBinaryFile), 1, p_file);
		if (file)
		{
			GameObject* obj = new GameObject(*file);
			Controller::GetInstance()->GetMainWnd()->BitmapGetInsert(obj->GetFileName());
			AddItemToTree(0, (LPTSTR)_fileName, m_objectRoot, TVI_LAST, (LPARAM)obj);
			switch (obj->GetObjCode())
			{
			case ObjectType::PlayerObj:
				Controller::GetInstance()->GetMainWnd()->SetPlayerObject(obj);
				break;
			
			case ObjectType::NefendesObj:
				Controller::GetInstance()->GetMainWnd()->SetNefendesObject(obj);
				break;
			
			case ObjectType::GhostObj:
				Controller::GetInstance()->GetMainWnd()->SetGhostObject(obj);
				break;
			
			case ObjectType::KumaObj:
				Controller::GetInstance()->GetMainWnd()->SetKumaObject(obj);
				break;
			}
		}
		delete file;
	}
}

LRESULT TreeViewWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	try
	{
		PAINTSTRUCT ps;
		HDC hdc;
		switch (message)
		{
		case WM_CREATE:
			break;

		case WM_KEYUP:
			break;

		case WM_KEYDOWN:
			break;

		case WM_TIMER:
			break;

		case WM_SIZE:
			break;

		case WM_COMMAND:
			break;

		case WM_LBUTTONDOWN:
			break;

		case WM_MOUSEMOVE:
			break;

		case WM_LBUTTONUP:
			break;

		case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code)
			{
			case TVN_SELCHANGED:
			{
				break;
			}
			case NM_DBLCLK:
			{
				TVITEM tvi = { 0 };
				tvi.hItem = GetCusorSel();
				TreeView_GetItem(m_treeviewHwnd, &tvi);
				ResourceObj* rc = (ResourceObj*)tvi.lParam;
				if (rc)
					Controller::GetInstance()->GetMainWnd()->TreeViewClickEventBind(rc);
			}
			case TVN_ITEMCHANGING:
			{
				break;
			}

			case TVM_GETNEXTITEM:
			{
				break;
			}

			case TVM_GETITEM:
			{
				break;
			}

			case TVN_KEYDOWN:
			{
			/*	LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)lParam;

				if (ptvkd->wVKey == 46)
				{
					TVITEM tvi = { 0 };
					tvi.hItem = GetCusorSel();
					TreeView_GetItem(m_treeviewHwnd, &tvi);
					ResourceObj* rc = (ResourceObj*)tvi.lParam;
					if (rc)
					{
						Controller::GetInstance()->GetMainWnd()->ClearAnimation();
					}
				}
				else if (ptvkd->wVKey == 83)
				{
					TVITEM tvi = { 0 };
					tvi.hItem = GetCusorSel();
					TreeView_GetItem(m_treeviewHwnd, &tvi);
					ResourceObj* rc = (ResourceObj*)tvi.lParam;
					if (rc)
					{
						Controller::GetInstance()->GetMainWnd()->SaveItem(rc);
					}
				}*/
				break;
			}
			}
			break;

		case WM_PAINT:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}
	catch (int error)
	{
		return -1;
	}
}

