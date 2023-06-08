#pragma once
#include <vector>
#include <queue>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "Wnd.h"
#include <commctrl.h>
#include <iostream>
#include <d2d1.h>
#include <dwrite.h>
#include "ResourceObj.h"
#include "Types.h"
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#include <wincodec.h>
#include "D2D1Core.h"
#include "D2D1Wnd.h"
#include <memory>
void GetFileExp(IN TCHAR* _fileName, OUT TCHAR* _ret);
void GetFileList(std::vector<TCHAR*>& _fileList, const TCHAR* _rootPath);