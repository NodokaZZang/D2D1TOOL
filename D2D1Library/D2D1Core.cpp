#include "pch.h"
#include "Bitmap.h"
#include "D2D1Core.h"
#include "Sprite.h"
D2D1Core* D2D1Core::m_instance = nullptr;
void D2D1Core::Init()
{
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pWICFactory));
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2D1Factory);
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&g_pD2D1DWriteFactory));
}

void D2D1Core::CreateRenderTarget(HWND _hwnd, ID2D1HwndRenderTarget** _rt)
{
	RECT rc;
	GetClientRect(_hwnd, &rc);
	g_pD2D1Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(_hwnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		_rt
	);
}

void D2D1Core::CreateRenderTarget(ID2D1HwndRenderTarget* _rt, ID2D1BitmapRenderTarget** _crt)
{
	_rt->CreateCompatibleRenderTarget(_crt);
}

D2D1Core* D2D1Core::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new D2D1Core();
	return m_instance;
}

Bitmap* D2D1Core::LoadBitmapByFileName(const PCWSTR _fileName)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;

	// Load Image
	hr = g_pWICFactory->CreateDecoderFromFilename(
		_fileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (FAILED(hr))
		return nullptr;

	// 0 index Frame get
	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr))
		return nullptr;
	void* memory = ConvertFrameToBitmap(pFrame);
	// MakeConverter
	IWICFormatConverter* pConverter = nullptr;
	hr = g_pWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr))
		return nullptr;

	hr = pConverter->Initialize(pFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeCustom);
	UINT width = 0;
	UINT height = 0;
	pConverter->GetSize(&width, &height);

	if (FAILED(hr))
		return nullptr;

	ID2D1Bitmap* temp;
	hr = g_pD2D1RenderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		&temp
	);

	if (FAILED(hr))
		return nullptr;

	if (pConverter) pConverter->Release();
	if (pFrame) pFrame->Release();
	if (pDecoder) pDecoder->Release();

	Bitmap* b = new Bitmap(temp, static_cast<BYTE*>(memory), width, height, 4);
	b->SetFileName(_fileName);
	return b;
}

Bitmap* D2D1Core::LoadBitmapByFileName(ID2D1HwndRenderTarget** _rt, const PCWSTR _fileName)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;

	// Load Image
	hr = g_pWICFactory->CreateDecoderFromFilename(
		_fileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (FAILED(hr))
		return nullptr;

	// 0 index Frame get
	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr))
		return nullptr;
	void* memory = ConvertFrameToBitmap(pFrame);
	// MakeConverter
	IWICFormatConverter* pConverter = nullptr;
	hr = g_pWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr))
		return nullptr;

	hr = pConverter->Initialize(pFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeCustom);
	UINT width = 0;
	UINT height = 0;
	pConverter->GetSize(&width, &height);

	if (FAILED(hr))
		return nullptr;

	ID2D1Bitmap* temp;
	hr = (*_rt)->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		&temp
	);

	if (FAILED(hr))
		return nullptr;

	if (pConverter) pConverter->Release();
	if (pFrame) pFrame->Release();
	if (pDecoder) pDecoder->Release();

	Bitmap* b = new Bitmap(temp, static_cast<BYTE*>(memory), width, height, 4);
	b->SetFileName(_fileName);
	return b;
}

void D2D1Core::SaveFile(Bitmap* _bitmap, const PCWSTR _fileName)
{
	HRESULT hr = S_OK;
	ID2D1Factory* pD2DFactory = NULL;
	IWICBitmap* pWICBitmap = NULL;
	ID2D1RenderTarget* pRT = NULL;
	IWICBitmapEncoder* pEncoder = NULL;
	IWICBitmapFrameEncode* pFrameEncode = NULL;
	IWICStream* pStream = NULL;

	BYTE* memory = _bitmap->GetMemory();
	const int widht = _bitmap->GetWidht();
	const int height = _bitmap->GetHeight();

	HRESULT rs = g_pWICFactory->CreateBitmapFromMemory(widht, height, GUID_WICPixelFormat32bppPBGRA, widht * 4, (widht) * (height) * 4, memory, &pWICBitmap);

	if (!pWICBitmap)
	{
		return;
	}

	rs = g_pWICFactory->CreateStream(&pStream);
	WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
	rs = pStream->InitializeFromFilename(_fileName, GENERIC_WRITE);
	rs = g_pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
	rs = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
	rs = pEncoder->CreateNewFrame(&pFrameEncode, NULL);
	rs = pFrameEncode->Initialize(NULL);
	rs = pFrameEncode->SetSize(widht, height);
	rs = pFrameEncode->SetPixelFormat(&format);
	rs = pFrameEncode->WriteSource(pWICBitmap, NULL);
	rs = pFrameEncode->Commit();
	rs = pEncoder->Commit();

	pWICBitmap->Release();
	pEncoder->Release();
	pFrameEncode->Release();
	pStream->Release();
}

void D2D1Core::SetFontFormat(IDWriteTextFormat** _format, const WCHAR _fontName[], const FLOAT _fontSize)
{
	g_pD2D1DWriteFactory->CreateTextFormat(
		_fontName, NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		_fontSize, L"en-us", _format);
}

void* D2D1Core::ConvertFrameToBitmap(IWICBitmapFrameDecode* frame)
{
	IWICBitmapSource* bitmapSource = frame;
	if (FAILED(WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, frame, &bitmapSource)))
	{
		// could not convert bitmap
		return nullptr;
	}

	UINT width = 0;
	UINT height = 0;
	if (FAILED(bitmapSource->GetSize(&width, &height)) || width == 0 || height == 0)
	{
		// could not get image size
		return nullptr;
	}
	const int buffSize = width * height * 4;
	BYTE* buffer = new BYTE[width * height * 4];
	if (FAILED(bitmapSource->CopyPixels(NULL, width * 4, buffSize, buffer)))
	{
		// could not get image size
		return nullptr;
	}

	bitmapSource->Release();

	// return CreateBitmap(width, height, 1, 32, &buffer[0]);
	return buffer;
}

void D2D1Core::CreateBitmap(const PCWSTR _fileName, Bitmap* _bitmap, Sprite* _obj)
{
	HRESULT hr = S_OK;
	ID2D1Factory* pD2DFactory = NULL;
	IWICBitmap* pWICBitmap = NULL;
	ID2D1RenderTarget* pRT = NULL;
	IWICBitmapEncoder* pEncoder = NULL;
	IWICBitmapFrameEncode* pFrameEncode = NULL;
	IWICStream* pStream = NULL;
	BYTE* m = _bitmap->GetMemory();

	const int width =	_bitmap->GetWidht();
	D2D1_RECT_F rect =  _obj->GetRect();
	const int mWidht = rect.right - rect.left;
	const int mHeight = rect.bottom - rect.top;
	BYTE* makeBuf = new BYTE[mWidht * mHeight * 4];
	int yPos = rect.top;
	for (int y = 0; y < mHeight; y++)
	{
		const int size = rect.left + (yPos++ * width);
		::memcpy(&makeBuf[y * mWidht * 4], &m[size * 4], mWidht * 4);
	}

	HRESULT rs = g_pWICFactory->CreateBitmapFromMemory(mWidht, mHeight, GUID_WICPixelFormat32bppPBGRA, mWidht * 4, (mWidht) * (mHeight) * 4, makeBuf, &pWICBitmap);

	if (!pWICBitmap)
	{
		return;
	}

	rs = g_pWICFactory->CreateStream(&pStream);
	WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
	rs = pStream->InitializeFromFilename(_fileName, GENERIC_WRITE);
	rs = g_pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
	rs = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
	rs = pEncoder->CreateNewFrame(&pFrameEncode, NULL);
	rs = pFrameEncode->Initialize(NULL);
	rs = pFrameEncode->SetSize(mWidht, mHeight);
	rs = pFrameEncode->SetPixelFormat(&format);
	rs = pFrameEncode->WriteSource(pWICBitmap, NULL);
	rs = pFrameEncode->Commit();
	pEncoder->Commit();
	pWICBitmap->Release();
	pEncoder->Release();
	pFrameEncode->Release();
	pStream->Release();
}