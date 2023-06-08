#include "pch.h"
#include "MainWnd.h"
#include "resource.h"
#include "Bitmap.h"
#include "Controller.h"
#include "Map.h"
#include "TreeViewWnd.h"
#include "Event.h"
#include "GameObject.h"
MainWnd::MainWnd(HINSTANCE _instance, const TCHAR _className[], const TCHAR _title[], DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_className = _className;
	m_titleName = _title;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	Init();
}

MainWnd::MainWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_className = _className;
	m_titleName = _title;
	m_x = _x;
	m_y = _y;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	m_hParent = _parent;
	Init();
}

void MainWnd::Init()
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
	wecx.lpszMenuName = MAKEINTRESOURCE(nullptr);
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

void MainWnd::CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance)
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
	Controller::GetInstance()->SetMainWnd(this);
	D2D1Core::GetInstance()->CreateRenderTarget(m_hwnd, &m_rt);
	D2D1Core::GetInstance()->CreateRenderTarget(m_rt, &m_brt);
	D2D1Core::GetInstance()->SetFontFormat(&m_textFormat, L"나눔고딕", 15.0f);
	SetBrush(D2D1::ColorF::Black);
	SetBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.9f), &m_brush1);
	SetBrush(D2D1::ColorF(D2D1::ColorF::Pink, 0.9f), &m_brush2);
	SetBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 0.9f), &m_brush3);
	SetBrush(D2D1::ColorF(D2D1::ColorF::Purple, 0.9f), &m_brush4);
	SetBrush(D2D1::ColorF(D2D1::ColorF::Orange, 0.9f), &m_brush5);
}


void MainWnd::Resize(UINT _width, UINT _height) 
{
	if (m_rt)
	{
		m_height = _height;
		m_width = _width;
		D2D1_SIZE_U resize = { m_width, m_height };
		m_rt->Resize(resize);
	}
}

void MainWnd::Render()
{
	m_brt->BeginDraw();
	m_brt->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
	ID2D1Bitmap* bitmap;
	if (m_bitmap)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		FLOAT width = (float)(rc.right - rc.left);
		FLOAT height = (float)(rc.bottom - rc.top);

		if (width >= m_bitmap->GetWidht())
			width = m_bitmap->GetWidht();

		if (height >= m_bitmap->GetHeight())
			height = m_bitmap->GetHeight();

		int leftPos = 0 + m_scrollX;
		int topPos = 0 + m_scrollY;

		int rightPos = 0 + width + m_scrollX;
		int bottomPos = 0 + height + m_scrollY;

		D2D1_RECT_F source = D2D1::RectF(0, 0, m_bitmap->GetWidht(), m_bitmap->GetHeight());
		D2D1_RECT_F dest = D2D1::RectF(0 + m_scrollX, 0 + m_scrollY, m_bitmap->GetWidht() + m_scrollX, m_bitmap->GetHeight() + m_scrollY);
		m_brt->DrawBitmap(m_bitmap->GetBitmap(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
	}


	if (m_gridState)
		GridRender();


	if (m_gameObjBitmap && m_target)
		GameObjRender();



	m_brt->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::Size(m_magnification, m_magnification)));
	m_brt->EndDraw();
	m_brt->GetBitmap(&bitmap);

	m_rt->BeginDraw();

	if (bitmap)
		m_rt->DrawBitmap(bitmap);

	m_rt->EndDraw();
}


BOOL MainWnd::NewMapDiallog(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		ShowWindow(hwndDig, NULL);
		UpdateWindow(hwndDig);
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDOK:
		{
			UINT XSize = GetDlgItemInt(hwndDig, IDC_X, NULL, 100);
			UINT YSize = GetDlgItemInt(hwndDig, IDC_Y, NULL, 100);
			Controller::GetInstance()->GetMainWnd()->GridUnSet();
			Controller::GetInstance()->GetMainWnd()->GridSet(XSize, YSize, 16);
			Controller::GetInstance()->GetMainWnd()->FileOpenProc();
			EndDialog(hwndDig, TRUE);
		}
		break;

		case IDCANCEL:
			EndDialog(hwndDig, 0);
			return TRUE;
		}
		InvalidateRect(hwndDig, NULL, TRUE);

	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwndDig, &ps);
		EndPaint(hwndDig, &ps);
	}
	}
	return FALSE;
}

void MainWnd::GridSet(int _xSize, int _ySzie, int _width)
{
	m_gridXSize = _xSize;
	m_gridYSize = _ySzie;
	m_gridWidth = _width;
	m_gridState = true;
	const int ySize = _ySzie;
	const int xSize = _xSize;
	m_mapData = new int* [ySize];
	for (int i = 0; i < ySize; i++)
	{
		m_mapData[i] = new int[xSize];
		memset(m_mapData[i], 0, xSize * 4);
	}
}

void MainWnd::GridUnSet()
{
	if (m_mapData)
	{
		for (int i = 0; i < m_gridYSize; i++)
			if (m_mapData[i])
				delete[] m_mapData[i];
		
		delete[] m_mapData;
	}
	
	m_gridXSize = 0;
	m_gridYSize = 0;
	m_gridWidth = 0;
	m_gridState = false;
}

void MainWnd::TreeViewClickEventBind(ResourceObj* _obj)
{	
	m_target = nullptr;

	switch (_obj->GetResourceType())
	{

	case MAP:
	{
		EreaseRender();
		Map* map = reinterpret_cast<Map*>(_obj);
		GridSet(map->GetXSize(), map->GetYSize(), 16);
		map->copyMapData(m_mapData);

		if (m_bitmapDB.find(map->GetFileName()) != m_bitmapDB.end())
		{
			m_bitmap = m_bitmapDB.find(map->GetFileName())->second;

			RECT size = GetClientSizeRect();
			SetScrollRange(m_scroll,
				SB_CTL,
				0,
				((m_bitmap->GetHeight() * m_magnification) - size.bottom) / m_magnification,
				TRUE);
		}
		else
		{
			TCHAR szUniCode[256] = { 0, };
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, map->GetImgFileName(), strlen(map->GetImgFileName()), szUniCode, strlen(map->GetImgFileName()));
			Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, szUniCode);
			m_bitmapDB.insert({ map->GetFileName(), bitmap });
			m_bitmap = bitmap;

			RECT size = GetClientSizeRect();
			SetScrollRange(m_scroll,
				SB_CTL,
				0,
				((m_bitmap->GetHeight() * m_magnification) - size.bottom) / m_magnification,
				TRUE);
		}
		break;
	}

	case EVENT:
		break;


	case GAME_OBJECT:
	{
		if (m_mapData == nullptr)
			break;
		GameObject* gameObj = reinterpret_cast<GameObject*>(_obj);
		m_gameObjBitmap =  BitmapGetInsert(gameObj->GetFileName());
		m_target = gameObj;
		break;
	}	
	}
}

Bitmap* MainWnd::BitmapGetInsert(char* _fileName) 
{
	if (m_bitmapDB.find(_fileName) != m_bitmapDB.end())
	{
		return m_bitmapDB.find(_fileName)->second;
	}
	else
	{
		TCHAR szUniCode[256] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _fileName, strlen(_fileName), szUniCode, strlen(_fileName));
		Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, szUniCode);
		m_bitmapDB.insert({ _fileName, bitmap });
		return bitmap;
	}

}

void MainWnd::Insert(char* _fileName)
{
	TCHAR szUniCode[256] = { 0, };
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _fileName, strlen(_fileName), szUniCode, strlen(_fileName));
	Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, szUniCode);
	m_bitmapDB.insert({ _fileName, bitmap });
	m_gameObjBitmap = bitmap;
}

void MainWnd::MenuBind(int _menu)
{
	switch (_menu)
	{
	case ID_NEW_MAP:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), m_hwnd, (DLGPROC)NewMapDiallog);
		break;

	case ID_FILE_SAVEMAP:
		SaveFile();
		break;


	case ID_TOOL_100:
		m_magnification = 1.0f;
		if (m_bitmap)
		{
			RECT size = GetClientSizeRect();
			SetScrollRange(m_scroll,
				SB_CTL,
				0,
				((m_bitmap->GetHeight() * m_magnification) - size.bottom) / m_magnification,
				TRUE);
		}
		break;


	case ID_TOOL_200:
		m_magnification = 2.0f;
		if (m_bitmap)
		{
			RECT size = GetClientSizeRect();
			SetScrollRange(m_scroll,
				SB_CTL,
				0,
				((m_bitmap->GetHeight() * m_magnification) - size.bottom) / m_magnification,
				TRUE);
		}
		break;


	case ID_TOOL_400:
		m_magnification = 4.0f;
		if (m_bitmap)
		{
			RECT size = GetClientSizeRect();
			SetScrollRange(m_scroll,
				SB_CTL,
				0,
				((m_bitmap->GetHeight() * m_magnification) - size.bottom) / m_magnification,
				TRUE);
		}
		break;
	}
}

void MainWnd::FileOpenProc()
{
	TCHAR* filePath = Wnd::FileOpen();
	TCHAR filePathCopy[256];
	if (filePath)
	{
		_tcscpy_s(filePathCopy, filePath);
		MainWnd::EraseBitmap();
		Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, filePathCopy);

		if (bitmap)
		{
			SetBitmap(bitmap);

			RECT size = GetClientSizeRect();
			SetScrollRange(m_scroll,
				SB_CTL,
				0,
				((m_bitmap->GetHeight() * m_magnification) - size.bottom) / m_magnification,
				TRUE);
		}
	}
}

void MainWnd::SaveFile()
{
	TCHAR* _filePath = FileSave();
	TCHAR filePath[256];
	TCHAR exp[256];

	if (_filePath == nullptr)
		return;

	_tcscpy_s(filePath, _filePath);

	if (filePath)
	{
		GetFileExp(filePath, exp);
		if (0 == _tcscmp(exp, _T("map")))
		{
			MapBinaryFileSave(filePath);
		}
		else if (0 == _tcscmp(exp, _T("obj")))
		{
			
		}
		else
		{

		}
	}
}


void MainWnd::Vscroll(WPARAM wParam, LPARAM lParam)
{
	int xDelta = 0;
	int yDelta;     // yDelta = new_pos - current_pos 
	int yNewPos;    // new position 
	SCROLLINFO si;

	if (m_bitmap)
	{
		switch (LOWORD(wParam))
		{
			// User clicked the scroll bar shaft above the scroll box. 
		case SB_PAGEUP:
			yNewPos = m_scrollY + 16;
			break;

			// User clicked the scroll bar shaft below the scroll box. 
		case SB_PAGEDOWN:
			yNewPos = m_scrollY - 16;
			break;

			// User clicked the top arrow. 
		case SB_LINEUP:
			yNewPos = m_scrollY + 16;
			break;

			// User clicked the bottom arrow. 
		case SB_LINEDOWN:
			yNewPos = m_scrollY - 16;
			break;

			// User dragged the scroll box. 
		case SB_THUMBPOSITION:
			yNewPos = HIWORD(wParam) * -1;
			break;

		default:
			yNewPos = m_scrollY;
		}

		RECT rect;
		GetClientRect(m_hwnd, &rect);
		const int maxHeight = ((m_bitmap->GetHeight() * m_magnification) - rect.bottom) / m_magnification;

		// New position must be between 0 and the screen height. 
		yNewPos = max(-1 * maxHeight, yNewPos);
		yNewPos = min(0, yNewPos);

		// If the current position does not change, do not scroll.
		if (yNewPos == m_scrollY)
			return;


		// Determine the amount scrolled (in pixels). 
		yDelta = yNewPos - m_scrollY;

		// Reset the current scroll position. 
		m_scrollY = yNewPos;

		SetScrollPos(m_scroll,        // 스크롤 바의 윈도우 핸들 
			SB_CTL,               // 메인 윈도우에 부착된 표준 스크롤바 또는 별도의 스크롤바(SB_CTL) 지정 
			-1 * m_scrollY,               // 스크롤박스의 위치
			TRUE);      // 스크롤바의 범위를 변경한 후 다시 그릴 것인지 여부
	}
}

void MainWnd::MapBinaryFileSave(TCHAR* _path)
{
	FILE* p_file = NULL;
	char path[256] = "";
	char fileName[256] = "";
	char imgFileName[256] = "";
	char* ptr = NULL;
	TCHAR szUniCode[256] = {0,};
	WideCharToMultiByte(CP_ACP, 0, _path, 256, path, 256, NULL, NULL);

	if (m_mapData && m_bitmap != nullptr && 0 == fopen_s(&p_file, path, "wb"))
	{
		MapDataBinaryFile mapBinaryFileData;
		mapBinaryFileData.xSize = m_gridXSize;
		mapBinaryFileData.ySize = m_gridYSize;
		strcpy_s(mapBinaryFileData.fileName, path);
		WideCharToMultiByte(CP_ACP, 0, m_bitmap->GetFileName(), 256, imgFileName, 256, NULL, NULL);
		strcpy_s(mapBinaryFileData.imgFileName, imgFileName);
	
		for (int y = 0; y < m_gridYSize; y++) 
			for (int x = 0; x < m_gridXSize; x++)
				mapBinaryFileData.mapData[y][x] = m_mapData[y][x];

		fwrite(&mapBinaryFileData, sizeof(MapDataBinaryFile), 1, p_file);
		fclose(p_file);

	
		ptr = strrchr(path, '\\');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

		if (ptr == NULL)
			strcpy_s(fileName, path);
		else
			strcpy_s(fileName, ptr + 1); // 포인터에 +1을 더하여 파일이름만 추출


		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, strlen(fileName), szUniCode, strlen(fileName));

		Map* map = new Map(mapBinaryFileData);
		HTREEITEM treeItem
			= 
			Controller::GetInstance()->
			GetTreeViewWnd()->
			AddItemToTree(0, szUniCode, Controller::GetInstance()->GetTreeViewWnd()->GetMapRoot(), TVI_LAST, (LPARAM)map);
	
	}
}

void MainWnd::EreaseRender()
{
	GridUnSet();
	m_bitmap = nullptr;
}

void MainWnd::ClickEvent(int _x, int _y)
{
	if (m_mapData == nullptr)
		return;
	Pos mapIndex = GetMousePosXYMap(_x, _y);

	if (mapIndex.x < 0 || mapIndex.y < 0 || mapIndex.x >= m_gridXSize || mapIndex.y >= m_gridYSize)
		return;

	TVITEM tvi = { 0 };	
	tvi.hItem = Controller::GetInstance()->GetTreeViewWnd()->GetCusorSel();
	TreeView_GetItem(Controller::GetInstance()->GetTreeViewWnd()->GetTreeViewHwnd(), &tvi);
	ResourceObj* rs = (ResourceObj*)tvi.lParam;

	switch (rs->GetResourceType())
	{
	case EVENT:
	{
		Event* eventObject = reinterpret_cast<Event*>(rs);
		EventHandler(eventObject, mapIndex);
		break;
	}

	case GAME_OBJECT:
	{
		GameObject* gom = reinterpret_cast<GameObject*>(rs);
		const int len = ( gom->GetWidth() - gom->GetXPos() ) / 2;
		Pos mapLeft = GetMousePosXYMap(_x - len, _y);
		Pos mapRight = GetMousePosXYMap(_x + len, _y);


		switch (gom->GetObjCode())
		{
		case ObjectType::PlayerObj:
			m_mapData[mapIndex.y][mapIndex.x] = Player;
			break;

		case ObjectType::NefendesObj:
			m_mapData[mapIndex.y][mapIndex.x] = Nefendes;
			for (int i = mapLeft.x; i <= mapRight.x; i++)
				if (mapIndex.x != i)
					m_mapData[mapIndex.y][i] = NefendesRect;
			break;

		case ObjectType::GhostObj:
			m_mapData[mapIndex.y][mapIndex.x] = Ghost;
			for (int i = mapLeft.x; i <= mapRight.x; i++)
				if (mapIndex.x != i)
					m_mapData[mapIndex.y][i] = GhostRect;
			break;

		case ObjectType::KumaObj:
			m_mapData[mapIndex.y][mapIndex.x] = Kuma;
			break;

		}
		break;
	}

	} 
}

void MainWnd::EventHandler(Event* _eventObj, const Pos& _pos)
{
	switch (_eventObj->GetEventType())
	{
	case EMPTY:
		m_mapData[_pos.y][_pos.x] = EMPTY;
		break;
		
	case WALL:
		m_mapData[_pos.y][_pos.x] = WALL;
		break;

	case NefendesRect:
		m_mapData[_pos.y][_pos.x] = NefendesRect;
		break;

	case GhostRect:
		m_mapData[_pos.y][_pos.x] = GhostRect;
		break;

	case KumaRect:
		m_mapData[_pos.y][_pos.x] = KumaRect;
		break;
	} 
}

void MainWnd::ObjHandler(GameObject* _obj, const Pos& _pos)
{
	switch (_obj->GetObjCode())
	{
	case ObjectType::PlayerObj: 
		m_mapData[_pos.y][_pos.x] = Player;
		break;

	case ObjectType::NefendesObj: 
		m_mapData[_pos.y][_pos.x] = Nefendes;
		break;

	case ObjectType::GhostObj: 
		m_mapData[_pos.y][_pos.x] = Ghost;
		break;

	case ObjectType::KumaObj: 
		m_mapData[_pos.y][_pos.x] = Kuma;
		break;

	}
}

void MainWnd::ObjHandler(EVENT_TYPE _type, const Pos& _pos)
{
	if (m_mapData[_pos.y][_pos.x] == EMPTY)
		m_mapData[_pos.y][_pos.x] = _type;
}

void MainWnd::GridRender()
{
	RECT size = GetClientSizeRect();
	

	for (int y = 0; y < m_gridYSize; y++)
	{
		for (int x = 0; x < m_gridXSize; x++)
		{			
			D2D1_RECT_F rect = { (x * m_gridWidth) + m_scrollX, (y * m_gridWidth) + m_scrollY ,((x + 1) * m_gridWidth) + m_scrollX, ((y + 1) * m_gridWidth)+m_scrollY };

			switch (m_mapData[y][x])
			{

			case EMPTY:
				break;

			case WALL:
				m_brt->FillRectangle(rect, m_brush1);
				break;

			case Player:
				GameObjGridRender(rect, Player);
				break;

			case Nefendes:
				GameObjGridRender(rect, Nefendes);
				break;

			case NefendesRect:
				m_brt->FillRectangle(rect, m_brush2);
				break;
		
			case Ghost:
				GameObjGridRender(rect, Ghost);
				break;

			case GhostRect:
				m_brt->FillRectangle(rect, m_brush3);
				break;

			case Kuma:
				GameObjGridRender(rect, Kuma);
				break;

			case KumaRect:
				m_brt->FillRectangle(rect, m_brush4);
				break;
			}
			m_brt->DrawRectangle(rect, m_brush);
		}
	}
}

void MainWnd::GameObjRender()
{
	int left = ((m_endMousePosX / m_magnification)- abs(m_target->GetPivotPos().x - m_target->GetXPos())) ;
	int right = ((m_endMousePosX / m_magnification) + abs(m_target->GetPivotPos().x - m_target->GetWidth())) ;
	int top = ((m_endMousePosY / m_magnification)- abs(m_target->GetPivotPos().y - m_target->GetYPos())) ;
	int bottom = ((m_endMousePosY / m_magnification)+ abs(m_target->GetPivotPos().y - m_target->GetHeight())) ;

	int s_left = m_target->GetXPos() ;
	int s_top = m_target->GetYPos() ;
	int s_right = m_target->GetWidth() ;
	int s_bttom = m_target->GetHeight() ;

	D2D1_RECT_F dest = { left, top, right, bottom };
	D2D1_RECT_F source = { s_left, s_top, s_right, s_bttom };
	m_brt->DrawBitmap(m_gameObjBitmap->GetBitmap(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
}

void MainWnd::GameObjGridRender(const D2D1_RECT_F& rect, EVENT_TYPE _type)
{
	GameObject* temp = nullptr;
	switch (_type)
	{
	case EMPTY:
		break;
	case WALL:
		break;
	case Player:
		temp = m_player;
		break;
	case Nefendes:
		temp = m_nefendes;
		break;
	case Ghost:
		temp = m_ghost;
		break;
	case Kuma:
		temp = m_kuma;
		break;
	default:
		break;
	}

	int startPosX = (((rect.right - rect.left) / 2) + rect.left);
	int startPosY = rect.bottom;

	int left = ((startPosX)-abs(temp->GetPivotPos().x - temp->GetXPos()));
	int right = ((startPosX)+abs(temp->GetPivotPos().x - temp->GetWidth()));
	int top = ((startPosY)-abs(temp->GetPivotPos().y - temp->GetYPos()));
	int bottom = ((startPosY)+abs(temp->GetPivotPos().y - temp->GetHeight()));

	int s_left = temp->GetXPos();
	int s_top = temp->GetYPos();
	int s_right = temp->GetWidth();
	int s_bttom = temp->GetHeight();

	D2D1_RECT_F dest = { left, top, right, bottom };
	D2D1_RECT_F source = { s_left, s_top, s_right, s_bttom };
	m_brt->DrawBitmap(BitmapGetInsert(temp->GetFileName())->GetBitmap(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
}

void MainWnd::ResourceLoad()
{
	TCHAR* _filePath = FileOpen();
	TCHAR filePath[256];
	TCHAR exp[256];
	if (_filePath == nullptr)
		return;

	_tcscpy_s(filePath, _filePath);

	if (filePath)
	{
		GetFileExp(filePath, exp);

		if (0 == _tcscmp(exp, _T("spr"))) 
		{
			// spirte 일때
		}
		else if (0 == _tcscmp(exp, _T("png")))
		{
			// png 일떄 
			m_bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, filePath);
		}
	}
}


void MainWnd::MouseWheel(WPARAM wParam, LPARAM lParam)
{
	if (m_bitmap)
	{
		if ((SHORT)HIWORD(wParam) > 0)
		{
			m_scrollY += 16;
		}
		else
		{
			m_scrollY -= 16;
		}

		RECT rect;
		GetClientRect(m_hwnd, &rect);
		const int maxHeight = ((m_bitmap->GetHeight() * m_magnification) - rect.bottom) / m_magnification;

		m_scrollY = max(-1 * maxHeight, m_scrollY);
		m_scrollY = min(0, m_scrollY);

		SetScrollPos(m_scroll,        // 스크롤 바의 윈도우 핸들 
			SB_CTL,               // 메인 윈도우에 부착된 표준 스크롤바 또는 별도의 스크롤바(SB_CTL) 지정 
			-1 * m_scrollY,               // 스크롤박스의 위치
			TRUE);      // 스크롤바의 범위를 변경한 후 다시 그릴 것인지 여부
	}

}

Pos MainWnd::GetMousePosXY(int _x, int _y)
{
	int x = (_x / m_magnification) - m_scrollX;
	int y = (_y / m_magnification) - m_scrollY;
	return { x, y };
}

Pos MainWnd::GetMousePosXYMap(int _x, int _y)
{
	int x = ((_x / m_magnification) - m_scrollX) / 16;
	int y = ((_y / m_magnification) - m_scrollY) / 16;
	return { x, y };
}


void MainWnd::KeyDown(WPARAM _param)
{
	switch (_param)
	{
	case VK_LEFT:
		m_scrollX += 16;
		break;

	case VK_RIGHT:
		m_scrollX -= 16;
		break;

	case VK_UP:
		m_scrollY += 16;
		break;

	case VK_DOWN:
		m_scrollY -= 16;
		break;
	}
}

LRESULT MainWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HMENU hMenu;
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
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_COMMAND:
		break;
	case WM_LBUTTONDOWN:
	{
		ClickEvent(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_MOUSEMOVE:
		m_endMousePosX = LOWORD(lParam);
		m_endMousePosY = HIWORD(lParam);
		break;

	case WM_LBUTTONUP:
		break;

	case WM_MOUSEWHEEL:
		MouseWheel(wParam, lParam);
		break;

	case WM_VSCROLL:
		break;

	case WM_PAINT:
		Render();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}