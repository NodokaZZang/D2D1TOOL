#include "pch.h"
#include "CorePch.h"
#pragma comment(lib, "winmm.lib")
#include "App.h"

int APIENTRY _tWinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstace, LPTSTR _lpCmdLine, int _nCmdShow)
{
	App app;
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	app.Init(_hInstance, _T("JGMapToolApp"), _T("JGMapToolApp"), width, height, _nCmdShow);

	return app.Dispatch();
}