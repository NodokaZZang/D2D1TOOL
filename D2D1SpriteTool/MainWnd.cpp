#include "pch.h"
#include "MainWnd.h"
#include "Text.h"
#include "Controller.h"
#include "bitmap.h"
#include "resource.h"
#include "Sprite.h"
#include "TreeViewWnd.h"
#include "PreViewWnd.h"
#include "Animation.h"
#include "ResourceObj.h"
MainWnd::MainWnd(HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow)
{
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

	D2D1_RECT_F mousePosrect = { 5.0f, (float)(m_height - 50), 500.0f, m_height };
	D2D1_RECT_F pixelPosrect = { 5.0f, (float)(m_height - 35), 500.0f, m_height };
	m_mousePosText = new Text(L"X: %d, Y: %d", mousePosrect, m_endMousePosX + m_scrollX, m_endMousePosY - m_scrollY);
	m_pixelPosText = new Text(L"pick color %x", pixelPosrect, 0);

	AddText(m_mousePosText);
	AddText(m_pixelPosText);
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
	D2D1Core::GetInstance()->CreateRenderTarget(m_hwnd, &m_rt);
	D2D1Core::GetInstance()->SetFontFormat(&m_textFormat, L"나눔고딕", 15.0f);
	SetBrush(D2D1::ColorF::Black);
	SetBrush(D2D1::ColorF::Red, &m_brush1);
	Controller::GetInstance()->SetMainWnd(this);
}

void MainWnd::Resize(UINT _width, UINT _height)
{
	if (m_rt)
	{
		m_height = _height;
		m_width = _width;
		D2D1_SIZE_U resize = { m_width, m_height };
		m_rt->Resize(resize);

		D2D1_RECT_F mousePosrect = { 5.0f, (float)(m_height - 50), 500.0f, m_height };
		D2D1_RECT_F pixelPosrect = { 5.0f, (float)(m_height - 35), 500.0f, m_height };
		if (m_mousePosText)
			m_mousePosText->SetRect(mousePosrect);
		if (m_pixelPosText)
			m_pixelPosText->SetRect(pixelPosrect);
	}
}

void MainWnd::Render()
{
	m_rt->BeginDraw();
	m_rt->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
	
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
		m_rt->DrawBitmap(m_bitmap->GetBitmap(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
	}

	// DRAG
	if (m_startMousePosX != NULL && m_startMousePosY != NULL)
	{
		D2D1_RECT_F dragZone = { m_startMousePosX, m_startMousePosY, m_endMousePosX, m_endMousePosY };
		m_rt->DrawRectangle(dragZone, m_brush, 1.0f);
	}
	
	for (TargetSprite& target : m_spriteVector)
	{
		D2D1_RECT_F rectCopy;
		D2D1_RECT_F rect = target.sprite->GetRect();
		rectCopy.left = rect.left + m_scrollX;
		rectCopy.top = rect.top + m_scrollY;
		rectCopy.right = rect.right + m_scrollX;;
		rectCopy.bottom = rect.bottom + m_scrollY;
		m_rt->DrawRectangle(rectCopy, m_brush, 1.0f);
		m_rt->DrawEllipse(D2D1::Ellipse({ target.sprite->GetPivot().x + m_scrollX, target.sprite->GetPivot().y + m_scrollY }, 2.f, 2.f), m_brush);
	}

	for (TargetSprite& target : m_targetrectVector)
	{
		D2D1_RECT_F rectCopy;
		Sprite* cs = target.sprite;
		D2D1_RECT_F rect = cs->GetRect();
		rectCopy.left = rect.left + m_scrollX;
		rectCopy.top = rect.top + m_scrollY;
		rectCopy.right = rect.right + m_scrollX;;
		rectCopy.bottom = rect.bottom + m_scrollY;
		m_rt->DrawRectangle(rectCopy, m_brush1, 1.0f);
	}

	// TEXT
	for (Text* text : m_textVector)
	{
		m_rt->DrawTextW(text->GetText(), wcslen(text->GetText()), m_textFormat, text->GetRect(), m_brush);
	}
	m_rt->EndDraw();
}

BOOL MainWnd::FPSSettingBox(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		ShowWindow(hwndDig, NULL);
		UpdateWindow(hwndDig);
		SetDlgItemInt(hwndDig, IDC_EDIT1, Controller::GetInstance()->GetFrame(), FALSE);
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON1:
		{
			UINT frame = GetDlgItemInt(hwndDig, IDC_EDIT1, NULL, 100);
			Controller::GetInstance()->SetFrame(frame);
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
			yNewPos = m_scrollY + 50;
			break;

			// User clicked the scroll bar shaft below the scroll box. 
		case SB_PAGEDOWN:
			yNewPos = m_scrollY - 50;
			break;

			// User clicked the top arrow. 
		case SB_LINEUP:
			yNewPos = m_scrollY + 20;
			break;

			// User clicked the bottom arrow. 
		case SB_LINEDOWN:
			yNewPos = m_scrollY - 20;
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
		const int maxHeight = m_bitmap->GetHeight() - rect.bottom;

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

void MainWnd::MouseWheel(WPARAM wParam, LPARAM lParam)
{
	if (m_bitmap)
	{
		if ((SHORT)HIWORD(wParam) > 0)
		{
			m_scrollY += 30;
		}
		else
		{
			m_scrollY -= 30;
		}

		RECT rect;
		GetClientRect(m_hwnd, &rect);
		const int maxHeight = m_bitmap->GetHeight() - rect.bottom;

		m_scrollY = max(-1 * maxHeight, m_scrollY);
		m_scrollY = min(0, m_scrollY);

		SetScrollPos(m_scroll,        // 스크롤 바의 윈도우 핸들 
			SB_CTL,               // 메인 윈도우에 부착된 표준 스크롤바 또는 별도의 스크롤바(SB_CTL) 지정 
			-1 * m_scrollY,               // 스크롤박스의 위치
			TRUE);      // 스크롤바의 범위를 변경한 후 다시 그릴 것인지 여부
	}
}

void MainWnd::MenuBind(int _menu, UINT& _nowMenu)
{
	HMENU hMenu;
	switch (_menu)
	{
		case ID_FILE_OPENFILE:
			FileOpenProc();
			break;

		case ID_TOOL_KEYCOLOR:
			KeyColorErase();
			break;

		case ID_TOOL_AUTOSPRITE:
			AutoSprite();
			break;

		case ID_TOOL_DRAGSPRITE:
			hMenu = GetMenu(m_hParent);
			if (_nowMenu == ID_TOOL_DRAGSPRITE)
			{
				CheckMenuRadioItem(hMenu, ID_TOOL_DRAGSPRITE, ID_MODE_SELECTIONMODE, NULL, MF_BYCOMMAND);
				_nowMenu = NULL;
				m_nowMenu = NULL;
			}
			else
			{
				CheckMenuRadioItem(hMenu, ID_TOOL_DRAGSPRITE, ID_MODE_SELECTIONMODE, ID_TOOL_DRAGSPRITE, MF_BYCOMMAND);
				_nowMenu = ID_TOOL_DRAGSPRITE;
				m_nowMenu = ID_TOOL_DRAGSPRITE;
			}
			break;

		case ID_TOOL_CLEAR:
			Clear();
			break;

		case ID_TOOL_CLEARALL:
			ClearAll();
			break;

		case ID_TOOL_MAKEANIMATION:
			MakeAnimation();
			break;

		case ID_TOOL_CLEARANIMATION:
			ClearAnimation();
			break;


		case ID_MODE_SELECTIONMODE:
		{
			hMenu = GetMenu(m_hParent);
			if (_nowMenu == ID_MODE_SELECTIONMODE)
			{
				CheckMenuRadioItem(hMenu, ID_TOOL_DRAGSPRITE, ID_MODE_SELECTIONMODE, NULL, MF_BYCOMMAND);
				_nowMenu = NULL;
				m_nowMenu = NULL;
			}
			else
			{
				CheckMenuRadioItem(hMenu, ID_TOOL_DRAGSPRITE, ID_MODE_SELECTIONMODE, ID_MODE_SELECTIONMODE, MF_BYCOMMAND);
				_nowMenu = ID_MODE_SELECTIONMODE;
				m_nowMenu = ID_MODE_SELECTIONMODE;
			}
			break;
		}

		case ID_SETTING_FPS:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), m_hwnd, (DLGPROC)FPSSettingBox);
			break;

		case ID_FILE_SAVEFILE:
			FileSaveProc();
			break;
	
		case ID_SPRITE_SAVESPRITE:
			SaveSprite();
			break;

		case ID_SPRITE_SAVEANIMATION:
			SaveAnimation();
			break;

		case ID_SPRITE_LOADSPRITE:
			LoadSprite();
			break;
	}
}

void MainWnd::FileOpenProc()
{
	TCHAR* filePath = FileOpen();
	TCHAR filePathCopy[256];
	if (filePath)
	{
		_tcscpy_s(filePathCopy, filePath);
		MainWnd::EraseBitmap();
		Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, filePathCopy);
	
		if (bitmap)
			SetBitmap(bitmap);

		RECT size = GetClientSizeRect();
		SetScrollRange(m_scroll,
			SB_CTL,
			0,
			bitmap->GetHeight() - size.bottom,
			TRUE);
	}
}

void MainWnd::KeyColorErase()
{
	if (m_bitmap)
	{
		DWORD* memory = (DWORD*)m_bitmap->GetMemory();
		const int width = m_bitmap->GetWidht();
		const int height = m_bitmap->GetHeight();
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (GetPosBitmapMemory(x, y) == m_keyColor)
					::memset((void*)(&memory[x + (y * width)]), 0, sizeof(DWORD));
			}
		}
		D2D1_RECT_U rect = { 0,0,width * sizeof(DWORD), height * sizeof(DWORD) };
		m_bitmap->GetBitmap()->CopyFromMemory(&rect, (void*)memory, width * sizeof(DWORD));
	}
}

void MainWnd::AutoSprite()
{
	if (m_bitmap == nullptr)
		return;

	if (m_discovered)
	{
		for (int y = 0; y < m_prevBitmapHeight; y++)
		{
			if (m_discovered[y])
			{
				delete[] m_discovered[y];
				m_discovered[y] = nullptr;
			}
		}
		delete[] m_discovered;
		m_discovered = nullptr;
	}

	const int widht = m_bitmap->GetWidht();
	const int height = m_bitmap->GetHeight();

	m_prevBitmapHeight = height;
	m_prevBitmapWidth = widht;

	m_discovered = new bool* [height];
	for (int y = 0; y < height; y++)
	{
		m_discovered[y] = new bool[widht];
		for (int x = 0; x < widht; x++)
		{
			m_discovered[y][x] = false;
		}
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < widht; x++)
		{
			if (m_discovered[y][x])
				continue;

			if (GetPosBitmapMemory(x, y) != 0)
			{
				BFS({ x, y });
			}
		}
	}
}

void MainWnd::DragSprite()
{
	if (m_bitmap == nullptr)
		return;

	Pos leftTop = GetMousePosXY(m_startMousePosX, m_startMousePosY);
	Pos rightBottom = GetMousePosXY(m_endMousePosX, m_endMousePosY);

	const int top = leftTop.y;
	const int left = leftTop.x;
	const int right = rightBottom.x;
	const int bottom = rightBottom.y;

	D2D1_RECT_F rect = { 0,0,0,0 };
	for (int y = top; y <= bottom; y++)
	{
		bool find = false;
		for (int x = left; x <= right; x++)
		{
			DWORD value = GetPosBitmapMemory(x, y);
			if (value != 0 && value != -1)
			{
				rect.top = y;
				find = true;
				break;
			}
		}
		if (find) break;
	}

	for (int y = bottom; y >= top; y--)
	{
		bool find = false;
		for (int x = left; x <= right; x++)
		{
			DWORD value = GetPosBitmapMemory(x, y);
			if (value != 0 && value != -1)
			{
				rect.bottom = y;
				find = true;
				break;
			}
		}
		if (find) break;
	}

	for (int x = left; x <= right; x++)
	{
		bool find = false;
		for (int y = top; y <= bottom; y++)
		{
			DWORD value = GetPosBitmapMemory(x, y);
			if (value != 0 && value != -1)
			{
				rect.left = x;
				find = true;
				break;
			}
		}
		if (find) break;
	}

	for (int x = right; x >= left; x--)
	{
		bool find = false;
		for (int y = top; y <= bottom; y++)
		{
			DWORD value = GetPosBitmapMemory(x, y);
			if (value != 0 && value != -1)
			{
				rect.right = x;
				find = true;
				break;
			}
		}
		if (find) break;
	}

	if (rect.left == 0 && rect.top == 0 && rect.right == 0 && rect.bottom == 0)
		return;

	D2D1_POINT_2F pivot = { ((rect.right - rect.left) / 2) + rect.left , rect.bottom };
	Sprite* sprite = new Sprite(rect, pivot);
	WCHAR buff[100];
	swprintf_s(buff, L"%d Sprite", TreeViewWnd::GetSpriteSq());
	HTREEITEM treeItem
		= Controller::GetInstance()->GetTreeViewWnd()->AddItemToTree(0, buff, Controller::GetInstance()->GetTreeViewWnd()->GetSpriteRoot(), TVI_LAST, (LPARAM)sprite);
	TargetSprite t = { m_spriteVector.size(), sprite, treeItem };
	m_spriteVector.push_back(t);
}

void MainWnd::Clear()
{
	for (auto& target : m_targetrectVector)
	{
		if (m_spriteVector[target.index].sprite)
		{
			TreeView_DeleteItem(Controller::GetInstance()->GetTreeViewWnd()->GetTreeViewHwnd(), m_spriteVector[target.index].treeKey);
			delete m_spriteVector[target.index].sprite;
			Controller::GetInstance()->GetPreViewWnd()->SetSprite(nullptr, nullptr);
		}
		m_spriteVector.erase(m_spriteVector.begin() + (target.index));
	}
}

void MainWnd::ClearAll()
{
	for (auto& sprite : m_spriteVector)
	{
		if (sprite.sprite)
		{
			TreeView_DeleteItem(Controller::GetInstance()->GetTreeViewWnd()->GetTreeViewHwnd(), sprite.treeKey);
			delete sprite.sprite;
			Controller::GetInstance()->GetPreViewWnd()->SetSprite(nullptr, nullptr);
		}
	}
	m_spriteVector.clear();
}

void MainWnd::ParseSprite()
{
	for (int i = m_spriteVector.size() - 1; i >= 0; i--)
	{
		if (CanParse(m_spriteVector[i].sprite))
		{
			TargetSprite t = { i, m_spriteVector[i].sprite, m_spriteVector[i].treeKey };
			m_targetrectVector.push_back(t);
		}
	}
}

void MainWnd::MakeAnimation()
{
	if (m_targetrectVector.size() != 0)
	{
		Animation* newAnimation = new Animation();
		for (auto& target : m_targetrectVector)
		{
			newAnimation->AddClip(target.sprite);
		}
		WCHAR buff[100];	
		swprintf_s(buff, L"%d Animation", TreeViewWnd::GetAnimationSq());
		HTREEITEM treeItem
			= Controller::GetInstance()->GetTreeViewWnd()->AddItemToTree(0, buff, Controller::GetInstance()->GetTreeViewWnd()->GetAnimationRoot(), TVI_LAST, (LPARAM)newAnimation);
	}
}

void MainWnd::ClearAnimation()
{
	TVITEM tvi = { 0 };
	tvi.hItem = Controller::GetInstance()->GetTreeViewWnd()->GetCusorSel();
	TreeView_GetItem(Controller::GetInstance()->GetTreeViewWnd()->GetTreeViewHwnd(), &tvi);
	ResourceObj* rs = (ResourceObj*)tvi.lParam;
	TreeView_DeleteItem(Controller::GetInstance()->GetTreeViewWnd()->GetTreeViewHwnd(), tvi.hItem);
	delete rs;
	Controller::GetInstance()->GetPreViewWnd()->SetSprite(nullptr, nullptr);
}

void MainWnd::SelectSprite()
{
	for (int i = m_spriteVector.size() - 1; i >= 0; i--)
	{
		if (CanSelect(m_spriteVector[i].sprite))
		{
			TargetSprite t = { i, m_spriteVector[i].sprite, m_spriteVector[i].treeKey };
			m_targetrectVector.push_back(t);
		}
	}
}

void MainWnd::FileSaveProc()
{
	if (m_bitmap)
	{
		TCHAR* filePath;
		filePath = FileSave();
		if (filePath)
		{
			D2D1Core::GetInstance()->SaveFile(m_bitmap, filePath);
		}
	}
}

void MainWnd::SaveSprite()
{
	TCHAR* filePath;
	filePath = FileSave();

	if (filePath)
	{
		FILE* p_file = NULL;
		char* ptr = NULL;
		char path[256] = "";
		char filename[256] = "";
		WideCharToMultiByte(CP_ACP, 0, filePath, 256, path, 256, NULL, NULL);
		ptr = strrchr(path, '\\');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

		if (ptr == NULL)
			strcpy_s(filename, path);
		else
			strcpy_s(filename, ptr + 1); // 포인터에 +1을 더하여 파일이름만 추출

		if (m_targetrectVector.size() != 0 && 0 == fopen_s(&p_file, filename, "wb"))
		{
			SpriteBinaryFileHeader header;
			WideCharToMultiByte(CP_ACP, 0, m_bitmap->GetFileName(), 256, header.fileName, 256, NULL, NULL);
			header.resourceType = SPRITE;
			header.spriteCount = m_targetrectVector.size();

			SpriteBinaryFileData* binaryStream = new SpriteBinaryFileData[m_targetrectVector.size()];
			for (int i = 0; i < m_targetrectVector.size(); i++)
			{
				Sprite* s = m_targetrectVector[i].sprite;
				SpriteBinaryFileData sf = { s->GetRect(), s->GetPivot() };
				binaryStream[i] = sf;
			}
			fwrite(&header, sizeof(SpriteBinaryFileHeader), 1, p_file);
			fwrite(binaryStream, sizeof(SpriteBinaryFileData), m_targetrectVector.size(), p_file);
			fclose(p_file);
			delete[] binaryStream;
		}
	}
}

void MainWnd::SaveAnimation()
{
	TCHAR* filePath;
	filePath = FileSave();

	if (filePath)
	{
		FILE* p_file = NULL;
		char* ptr = NULL;
		char path[256] = "";
		char filename[256] = "";

		WideCharToMultiByte(CP_ACP, 0, filePath, 256, path, 256, NULL, NULL);
		ptr = strrchr(path, '\\');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

		if (ptr == NULL)
			strcpy_s(filename, path);
		else
			strcpy_s(filename, ptr + 1); // 포인터에 +1을 더하여 파일이름만 추출

		if (m_targetrectVector.size() != 0 && 0 == fopen_s(&p_file, filename, "wb"))
		{
			SpriteBinaryFileHeader header;
			WideCharToMultiByte(CP_ACP, 0, m_bitmap->GetFileName(), 256, header.fileName, 256, NULL, NULL);
			header.resourceType = ANIMATION;
			header.spriteCount = m_targetrectVector.size();

			SpriteBinaryFileData* binaryStream = new SpriteBinaryFileData[m_targetrectVector.size()];
			for (int i = 0; i < m_targetrectVector.size(); i++)
			{
				Sprite* s = m_targetrectVector[i].sprite;
				SpriteBinaryFileData sf = { s->GetRect(), s->GetPivot() };
				binaryStream[i] = sf;
			}
			fwrite(&header, sizeof(SpriteBinaryFileHeader), 1, p_file);
			fwrite(binaryStream, sizeof(SpriteBinaryFileData), m_targetrectVector.size(), p_file);
			fclose(p_file);
			delete[] binaryStream;
		}
	}
}

void MainWnd::SaveItem(ResourceObj* _obj)
{
	TCHAR* filePath;
	filePath = FileSave();

	if (filePath)
	{
		FILE* p_file = NULL;
		char* ptr = NULL;
		char path[256] = "";
		char filename[256] = "";

		WideCharToMultiByte(CP_ACP, 0, filePath, 256, path, 256, NULL, NULL);
		ptr = strrchr(path, '\\');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

		if (ptr == NULL)
			strcpy_s(filename, path);
		else
			strcpy_s(filename, ptr + 1); // 포인터에 +1을 더하여 파일이름만 추출

		if (0 == fopen_s(&p_file, filename, "wb"))
		{
			SpriteBinaryFileHeader header;
			WideCharToMultiByte(CP_ACP, 0, m_bitmap->GetFileName(), 256, header.fileName, 256, NULL, NULL);

			switch (_obj->GetResourceType())
			{
			case SPRITE:
				{
					header.resourceType = SPRITE;
					header.spriteCount = 1;
					SpriteBinaryFileData* binaryStream = new SpriteBinaryFileData();
					binaryStream->rect = reinterpret_cast<Sprite*>(_obj)->GetRect();
					binaryStream->pivotPos = reinterpret_cast<Sprite*>(_obj)->GetPivot();
					fwrite(&header, sizeof(SpriteBinaryFileHeader), 1, p_file);
					fwrite(binaryStream, sizeof(SpriteBinaryFileData), header.spriteCount, p_file);
					delete binaryStream;
					break;			
				}
			case ANIMATION:
				{
					header.resourceType = ANIMATION;
					header.spriteCount = reinterpret_cast<Animation*>(_obj)->GetFrameCount();
					SpriteBinaryFileData* binaryStream = new SpriteBinaryFileData[header.spriteCount];

					for (int i = 0; i < header.spriteCount; i++)
					{
						Sprite* s = reinterpret_cast<Animation*>(_obj)->GetClips()[i];
						SpriteBinaryFileData sf = { s->GetRect(), s->GetPivot() };
						binaryStream[i] = sf;
					}

					fwrite(&header, sizeof(SpriteBinaryFileHeader), 1, p_file);
					fwrite(binaryStream, sizeof(SpriteBinaryFileData), header.spriteCount, p_file);
					delete[] binaryStream;
					break;
				}	
			}
			fclose(p_file);
		}
	}
}

void MainWnd::SaveGameObject(Sprite* _sprite)
{
	TCHAR* filePath;
	filePath = FileSave();

	if (filePath)
	{
		FILE* p_file = NULL;
		char* ptr = NULL;
		char path[256] = "";
		char filename[256] = "";

		WideCharToMultiByte(CP_ACP, 0, filePath, 256, path, 256, NULL, NULL);
		ptr = strrchr(path, '\\');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

		if (ptr == NULL)
			strcpy_s(filename, path);
		else
			strcpy_s(filename, ptr + 1); // 포인터에 +1을 더하여 파일이름만 추출

		if (0 == fopen_s(&p_file, filename, "wb"))
		{
			ObjDataBinaryFile* gameObjectBinaryFile = new ObjDataBinaryFile();
			WideCharToMultiByte(CP_ACP, 0, m_bitmap->GetFileName(), 256, gameObjectBinaryFile->fileName, 256, NULL, NULL);
			gameObjectBinaryFile->xPos = _sprite->GetRect().left;
			gameObjectBinaryFile->yPos = _sprite->GetRect().top;
			gameObjectBinaryFile->width = _sprite->GetRect().right;
			gameObjectBinaryFile->height = _sprite->GetRect().bottom;
			gameObjectBinaryFile->pivotPos = _sprite->GetPivot();
			gameObjectBinaryFile->objectCode = ObjectType::KumaObj;
			fwrite(gameObjectBinaryFile, sizeof(ObjDataBinaryFile), 1, p_file);
			delete gameObjectBinaryFile;
		}
		fclose(p_file);
	}
}

void MainWnd::CreateBitmap(ResourceObj* _obj)
{
	switch (_obj->GetResourceType())
	{
	case SPRITE:
		if (m_bitmap)
		{
			TCHAR* filePath;
			filePath = FileSave();
			TCHAR filePathReal[256];
			if (filePath == nullptr)
				return;

			_tcscpy_s(filePathReal, filePath);
			if (filePathReal)
			{
				D2D1Core::GetInstance()->CreateBitmap(filePathReal,m_bitmap,reinterpret_cast<Sprite*>(_obj));
			}
		}
		break;
	} 
}

void MainWnd::LoadSprite()
{
	TCHAR* filePath;
	filePath = FileOpen();
	if (filePath)
	{
		FILE* p_file = NULL;
		char* ptr = NULL;
		char path[256] = "";
		char filename[256] = "";
		WideCharToMultiByte(CP_ACP, 0, filePath, 256, path, 256, NULL, NULL);
		fopen_s(&p_file, path, "rb");
		if (p_file != NULL)
		{
			SpriteBinaryFileHeader* header = new SpriteBinaryFileHeader();
			fread(header, sizeof(SpriteBinaryFileHeader), 1, p_file);
			if (header) 
			{
				if (m_bitmap == nullptr)
				{
					TCHAR szUniCode[256] = { 0, };
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, header->fileName, strlen(header->fileName), szUniCode, 256);
					Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, szUniCode);

					if (bitmap)
						SetBitmap(bitmap);

					RECT size = GetClientSizeRect();
					SetScrollRange(m_scroll,
						SB_CTL,
						0,
						bitmap->GetHeight() - size.bottom,
						TRUE);
				}
				SpriteBinaryFileData* bStream = new SpriteBinaryFileData[header->spriteCount];
				fread(bStream, sizeof(SpriteBinaryFileData), header->spriteCount, p_file);
				switch (header->resourceType)
				{
				case SPRITE:
					for (int i = 0; i < header->spriteCount; i++)
					{
						Sprite* sprite = new Sprite(bStream[i].rect, bStream[i].pivotPos);
						WCHAR buff[100];
						swprintf_s(buff, L"%d Sprite", TreeViewWnd::GetSpriteSq());
						HTREEITEM treeItem
							= Controller::GetInstance()->GetTreeViewWnd()->AddItemToTree(0, buff, Controller::GetInstance()->GetTreeViewWnd()->GetSpriteRoot(), TVI_LAST, (LPARAM)sprite);
						TargetSprite t = { m_spriteVector.size(), sprite, treeItem };
						m_spriteVector.push_back(t);
					}
					break;
				case  ANIMATION:
				{
					Animation* ani = new Animation();
					for (int i = 0; i < header->spriteCount; i++)
					{
						Sprite* sprite = new Sprite(bStream[i].rect, bStream[i].pivotPos);
						ani->AddClip(sprite);				
					}
					WCHAR buff[100];
					swprintf_s(buff, L"%d Animation", TreeViewWnd::GetAnimationSq());
					HTREEITEM treeItem
						= Controller::GetInstance()->GetTreeViewWnd()->AddItemToTree(0, buff, Controller::GetInstance()->GetTreeViewWnd()->GetAnimationRoot(), TVI_LAST, (LPARAM)ani);				
				}
					break;
				}
				delete[] bStream;
			}		
			delete header;
		}
	}
}

void MainWnd::EraseBitmap()
{
	m_bitmap = nullptr;
	delete m_bitmap;
	m_targetrectVector.clear();
	ClearAll();
}

bool MainWnd::CanParse(Sprite* _sprite)
{
	Pos leftTop = GetMousePosXY(m_startMousePosX, m_startMousePosY);
	Pos rightBottom = GetMousePosXY(m_endMousePosX, m_endMousePosY);
	const int top = leftTop.y;
	const int left = leftTop.x;
	const int right = rightBottom.x;
	const int bottom = rightBottom.y;

	D2D1_RECT_F _rect = _sprite->GetRect();
	if (left > _rect.right || right < _rect.left || top > _rect.bottom || bottom < _rect.top)
		return false;
	else
		return true;
}

bool MainWnd::CanSelect(Sprite* _sprite)
{
	Pos xy = GetMousePosXY(m_startMousePosX, m_startMousePosY);
	D2D1_RECT_F _rect = _sprite->GetRect();

	for (auto& item : m_targetrectVector)
	{
		if (item.sprite->GetRect().left == _rect.left && item.sprite->GetRect().right == _rect.right && item.sprite->GetRect().top == _rect.top && item.sprite->GetRect().bottom == _rect.bottom)
		{
			return false;
		}
	}
	if (xy.x > _rect.right || xy.x < _rect.left || xy.y > _rect.bottom || xy.y < _rect.top)
		return false;
	else
		return true;
}

void MainWnd::KeyDown(WPARAM _param)
{
	switch (_param)
	{
	case VK_LEFT:
		m_scrollX -= 10;
		break;

	case VK_RIGHT:
		m_scrollX += 10;
		break;
	}
}

Pos MainWnd::GetMousePosXY(int _x, int _y)
{
	int x = (_x - m_scrollX);
	int y = (_y - m_scrollY);
	return { x, y };
}

void MainWnd::BFS(Pos _pos)
{
	int moveX[] = { 0, 1, 0, -1 };
	int moveY[] = { -1, 0, 1, 0 };
	m_discovered[_pos.y][_pos.x] = true;
	m_jobQueue.push(_pos);
	int leftPos = _pos.x;
	int topPos = _pos.y;
	int rightPos = _pos.x;
	int bottomPos = _pos.y;
	while (m_jobQueue.empty() == false)
	{
		Pos now = m_jobQueue.front();
		m_jobQueue.pop();

		for (int dir = 0; dir < 4; dir++)
		{
			Pos next = { now.x + moveX[dir], now.y + moveY[dir] };

			if (next.x < 0 || next.y < 0 || next.x >= m_bitmap->GetWidht() || next.y >= m_bitmap->GetHeight())
				continue;

			if (m_discovered[next.y][next.x])
				continue;

			if (GetPosBitmapMemory(next.x, next.y) == 0)
				continue;

			leftPos = min(leftPos, next.x);
			topPos = min(topPos, next.y);
			rightPos = max(rightPos, next.x);
			bottomPos = max(bottomPos, next.y);
			m_discovered[next.y][next.x] = true;
			m_jobQueue.push(next);
		}
	}
	D2D1_RECT_F rect = { leftPos,topPos, rightPos, bottomPos };
	D2D1_POINT_2F pivot = { ((rect.right - rect.left) / 2) + rect.left , rect.bottom };
	Sprite* sprite = new Sprite(rect, pivot);
	WCHAR buff[100];
	swprintf_s(buff, L"%d Sprite", TreeViewWnd::GetSpriteSq());
	HTREEITEM treeItem
		= Controller::GetInstance()->GetTreeViewWnd()->AddItemToTree(0, buff, Controller::GetInstance()->GetTreeViewWnd()->GetSpriteRoot(), TVI_LAST, (LPARAM)sprite);
	TargetSprite t = { m_spriteVector.size(), sprite, treeItem };
	m_spriteVector.push_back(t);

}

LRESULT MainWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HMENU hMenu;
	switch (message)
	{
	case WM_CREATE:
		hMenu = GetMenu(hWnd);
		break;

	case WM_KEYUP:
		break;

	case WM_KEYDOWN:
		break;

	case WM_TIMER:
		break;

	case WM_SIZE:
		Resize(wParam, lParam);
		break;

	case WM_COMMAND:
		break;

	case WM_LBUTTONDOWN:
	{
		m_startMousePosX = LOWORD(lParam);
		m_startMousePosY = HIWORD(lParam);
		Pos pos = GetMousePosXY(m_startMousePosX, m_startMousePosY);
		m_keyColor = GetPosBitmapMemory(pos.x, pos.y);
		if (m_keyColor == 4294967295)
			m_pixelPosText->Update(L"pick color FFFFFFFF");
		else if (m_keyColor != -1)
			m_pixelPosText->Update(L"pick color %0.8X", m_keyColor);
		break;
	}


	case WM_MOUSEMOVE:
	{
		m_endMousePosX = LOWORD(lParam);
		m_endMousePosY = HIWORD(lParam);
		m_mousePosText->Update(L"X: %d, Y: %d", m_endMousePosX + m_scrollX, m_endMousePosY - m_scrollY);
		break;
	}


	case WM_LBUTTONUP:
	{
		if (m_nowMenu == ID_TOOL_DRAGSPRITE)
		{
			m_targetrectVector.clear();
			DragSprite();
		}
		else if (m_nowMenu == ID_MODE_SELECTIONMODE)
		{
			if (m_spriteVector.size() != 0)
				SelectSprite();
		}
		else
		{
			m_targetrectVector.clear();
			if (m_spriteVector.size() != 0)
				ParseSprite();
		}

		m_startMousePosX = NULL;
		m_startMousePosY = NULL;
		break;
	}


	case WM_MOUSEWHEEL:
		MouseWheel(wParam, lParam);
		break;

	case WM_VSCROLL:
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
}

