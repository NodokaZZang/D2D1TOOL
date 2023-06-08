#pragma once
#include "pch.h"
class App
{
private:
	HINSTANCE				m_hInstance;
	TCHAR					m_className;
	TCHAR					m_classTitle;
	DWORD					m_widht;
	DWORD					m_height;

private:
	Wnd* m_MainWindow;

public:
	App() = default;
	~App() {};
	void	Init(HINSTANCE, const TCHAR[], const TCHAR[], DWORD, DWORD, int);
	int		Dispatch();
private:
public:
};

