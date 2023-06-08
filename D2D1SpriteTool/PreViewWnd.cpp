#include "pch.h"
#include "PreViewWnd.h"
#include "Controller.h"
#include "Sprite.h"
#include "Bitmap.h"
#include "Animation.h"
PreViewWnd::PreViewWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow)
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

void PreViewWnd::Init()
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
	wecx.lpfnWndProc = Wnd::WndProc;  // Á¤ÀûÇÔ¼ö
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

void PreViewWnd::CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance)
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
	D2D1Core::GetInstance()->SetFontFormat(&m_textFormat, L"³ª´®°íµñ", 15.0f);
	SetBrush(D2D1::ColorF::Red);
	SetBrush(D2D1::ColorF::Red, &m_brush1);
}


void PreViewWnd::SetSprite(ResourceObj* _sprite, Bitmap* _bitmap)
{
	m_target = nullptr;	
	m_bitmap = nullptr;
	m_target = _sprite;
	m_bitmap = _bitmap;
}

void PreViewWnd::RenderSprite(ResourceObj* _sprite)
{
	RECT crect;
	GetClientRect(m_hwnd, &crect);


	switch (_sprite->GetResourceType())
	{
		case SPRITE:
		{	
			Sprite* sprite = reinterpret_cast<Sprite*>(_sprite);
			RenderSpriteProc(sprite);

			D2D1_POINT_2F pivotPos = reinterpret_cast<Sprite*>(_sprite)->GetPivot();
			D2D1_RECT_F   rectPos = reinterpret_cast<Sprite*>(_sprite)->GetRect();

			WCHAR pivotPosBuf[100];
			WCHAR rectPosBufLeft[100];
			WCHAR rectPosBufRight[100];
			WCHAR rectPosBufTop[100];
			WCHAR rectPosBufBottom[100];

			RECT crect;
			GetClientRect(m_hwnd, &crect);

			swprintf_s(pivotPosBuf, L"Pivot X: %f, Y: %f ", pivotPos.x, pivotPos.y);
			swprintf_s(rectPosBufLeft, L"Pos Left: %f ", rectPos.left);
			swprintf_s(rectPosBufTop, L"Pos Top: %f ", rectPos.top);
			swprintf_s(rectPosBufRight, L"Pos Right: %f ", rectPos.right);
			swprintf_s(rectPosBufBottom, L"Pos Bottom: %f ", rectPos.bottom);

			m_rt->DrawTextW(pivotPosBuf, wcslen(pivotPosBuf), m_textFormat, D2D1::RectF(crect.left, crect.bottom - 40, crect.right, crect.bottom), m_brush);
			m_rt->DrawTextW(rectPosBufLeft, wcslen(rectPosBufLeft), m_textFormat, D2D1::RectF(crect.left, crect.bottom - 60, crect.right, crect.bottom), m_brush);
			m_rt->DrawTextW(rectPosBufTop, wcslen(rectPosBufTop), m_textFormat, D2D1::RectF(crect.left, crect.bottom - 80, crect.right, crect.bottom), m_brush);
			m_rt->DrawTextW(rectPosBufRight, wcslen(rectPosBufRight), m_textFormat, D2D1::RectF(crect.left, crect.bottom - 100, crect.right, crect.bottom), m_brush);
			m_rt->DrawTextW(rectPosBufBottom, wcslen(rectPosBufBottom), m_textFormat, D2D1::RectF(crect.left, crect.bottom - 120, crect.right, crect.bottom), m_brush);
			break;
		}


		case ANIMATION:
		{
			int currentTick = ::GetTickCount64();
			m_sumTick += currentTick - m_lastTick;
			if (Controller::GetInstance()->GetFrame() <= m_sumTick)
			{
				m_sumTick = 0;
				RenderSpriteProc(reinterpret_cast<Animation*>(m_target)->GetFrame());
			}
			else
			{
				RenderSpriteProc(reinterpret_cast<Animation*>(m_target)->GetFrameNow());
			}
			m_lastTick = currentTick;
			break;
		}
		
	}
}

void PreViewWnd::RenderSpriteProc(Sprite* _sprite)
{
	RECT crect;
	GetClientRect(m_hwnd, &crect);

	Sprite* sprite = reinterpret_cast<Sprite*>(_sprite);
	int widhtPadding = 125;
	int heighPadding = 150;
	D2D1_RECT_F rect = sprite->GetRect();
	D2D1_RECT_F source = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);

	if (crect.right <= (source.right - source.left) + widhtPadding)
		widhtPadding = 0;

	if (crect.bottom - 120 < (source.bottom - source.top) + heighPadding)
		heighPadding = 0;

	D2D1_RECT_F dest = D2D1::RectF(
		crect.left + widhtPadding
		, crect.top + heighPadding
		, crect.left + widhtPadding + (source.right - source.left)
		, crect.top + heighPadding + (source.bottom - source.top));
	m_rt->DrawBitmap(m_bitmap->GetBitmap(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
}

void PreViewWnd::Resize(UINT _width, UINT _height)
{

}

void PreViewWnd::Render()
{
	m_rt->BeginDraw();
	m_rt->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
	if (m_bitmap && m_target) 
		RenderSprite(m_target);


	m_rt->EndDraw();
}

LRESULT PreViewWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
