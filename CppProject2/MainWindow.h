#pragma once
#include <windows.h>
#include <strsafe.h>

#include <locale>
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <stdexcept>
#include <exception>

#define CREATECONSOLEBTN_ID 0x401
#define CLOSECONSOLEBTN_ID 0x402
#define COMBOBOX_ID 0x403

class MainWindow
{
	HINSTANCE _hInstance;
	HINSTANCE _hPrevInstance;
	LPSTR _lpCmdLine;
	int _nCmdShow;

	LPCTSTR lpzClass;
	HWND hWndMain;
	std::list<HANDLE> subThreads;

	HWND createConsoleBtn;
	HWND closeConsoleBtn;
	HWND comboBox;

	std::string tempstr;
public:
	MainWindow();
	MainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	MainWindow(MainWindow&&);
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(MainWindow&&);

	void onWmCreate(HWND hWnd);

	HWND getHWndMain() const;
	HWND getCreateConsoleBtn() const;
	HWND getCloseConsoleBtn() const;
	HWND getComboBox() const;

	static ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName, WNDPROC wndproc);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI threadMainFunc(LPVOID lpParam);

	static std::string utf8_encode(const std::wstring& wstr);
	static std::wstring utf8_decode(const std::string& str);
};

extern MainWindow mw;
