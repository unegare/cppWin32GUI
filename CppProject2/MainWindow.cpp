#include "MainWindow.h"

MainWindow mw;

MainWindow::MainWindow() {
	OutputDebugStringA("DEFAULT CONSTRUCTOR\n");
	std::ostringstream oss;
	oss << "DEFAULT CONSTRUCTOR: this == " << this << std::endl;
	OutputDebugStringA(oss.str().c_str());
}

MainWindow::MainWindow(MainWindow&& _mw) :
	_hInstance{std::move(_mw._hInstance)}, _hPrevInstance{std::move(_mw._hPrevInstance)},
	_lpCmdLine{ std::move(_mw._lpCmdLine) }, _nCmdShow{std::move(_mw._nCmdShow)},
	lpzClass{ std::move(_mw.lpzClass) }, hWndMain{std::move(_mw.hWndMain)},
	subThreads{std::move(_mw.subThreads)},
	createConsoleBtn{std::move(_mw.createConsoleBtn)},
	closeConsoleBtn{std::move(_mw.closeConsoleBtn)},
	comboBox{std::move(_mw.comboBox)},
	tempstr{std::move(_mw.tempstr)}
{
	OutputDebugStringA("MOVE CONSTRUCTOR\n");
}

MainWindow& MainWindow::operator= (MainWindow&& _mw) {
	if (&_mw == this) return *this;
	_hInstance = std::move(_mw._hInstance);
	_hPrevInstance = std::move(_mw._hPrevInstance);
	_lpCmdLine = std::move(_mw._lpCmdLine);
	_nCmdShow = std::move(_mw._nCmdShow);
	lpzClass = std::move(_mw.lpzClass);
	hWndMain = std::move(_mw.hWndMain);
	subThreads = std::move(_mw.subThreads);
	createConsoleBtn = std::move(_mw.createConsoleBtn);
	closeConsoleBtn = std::move(_mw.closeConsoleBtn);
	comboBox = std::move(_mw.comboBox);
	tempstr = std::move(_mw.tempstr);
	std::ostringstream oss;
	oss << __FUNCTION__ << ": &mw == " << &mw << std::endl;
	OutputDebugStringA(oss.str().c_str());
	std::cout << "MOVE operator=" << std::endl;
	return *this;
}

MainWindow::MainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) :
	_hInstance{ hInstance }, _hPrevInstance{hPrevInstance}, _lpCmdLine{lpCmdLine}, _nCmdShow{nCmdShow},
	lpzClass{ TEXT("My Window Class!") } {
	OutputDebugStringA("PROPER CONSTRUCTOR\n");
	if (!RegMyWindowClass(_hInstance, lpzClass, WndProc)) {
		OutputDebugString(TEXT("Registration failure\n"));
		std::ostringstream ss;
		ss << GetLastError() << std::endl;
		OutputDebugStringA(ss.str().c_str());
//
//		LPCTSTR lpszFunction = TEXT("funcName");
//		LPVOID lpMsgBuf;
//		LPVOID lpDisplayBuf;
//		DWORD dw = GetLastError(); 
//
//		FormatMessage(
//			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//			FORMAT_MESSAGE_FROM_SYSTEM |
//			FORMAT_MESSAGE_IGNORE_INSERTS,
//			NULL,
//			dw,
//			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//			(LPTSTR) &lpMsgBuf,
//			0, NULL );
//
//		// Display the error message and exit the process
//
//		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
//			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
//		StringCchPrintf((LPTSTR)lpDisplayBuf, 
//			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
//			TEXT("%s failed with error %d: %s"), 
//			lpszFunction, dw, lpMsgBuf); 
//		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
//
//		LocalFree(lpMsgBuf);
//		LocalFree(lpDisplayBuf);
//
		throw std::runtime_error("Registration failure"); 
	}
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int w = 500;
	int h = 300;
	int x = (screen_rect.right - w) / 2;
	int y = (screen_rect.bottom - h) / 2;

	hWndMain = CreateWindow(lpzClass, TEXT("Main Window"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y, w, h,
		NULL, NULL, _hInstance, NULL);
	if (!hWndMain) throw std::runtime_error("CreateWindow failure");
	std::ostringstream ss;
	ss << __FUNCTION__ << ": just after CreateWindow hWndMain: " << hWndMain << std::endl;
	OutputDebugStringA(ss.str().c_str());
	tempstr = ss.str();
}

ATOM MainWindow::RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName, WNDPROC wndproc) {
	WNDCLASS wcWindowClass = { 0 };
	wcWindowClass.lpfnWndProc = wndproc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.lpszClassName = lpzClassName;
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;

	return RegisterClass(&wcWindowClass);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static bool init = false;
	switch (message) {
	case WM_CREATE:
		if (!init) {
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			FILE* fDummy;
			freopen_s(&fDummy, "CONIN$", "r", stdin);
			freopen_s(&fDummy, "CONOUT$", "w", stdout);
			freopen_s(&fDummy, "CONOUT$", "w", stderr);
			std::cin.clear();
			std::cout.clear();
			std::cerr.clear();
			init = true;
		}
		std::cout << "mw.getHWndMain() returned: " << mw.getHWndMain() << std::endl;
		mw.onWmCreate(hWnd);
		break;
	case WM_LBUTTONUP:
		MessageBox(hWnd, TEXT("There's been a click!"), TEXT("en event"), 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case CREATECONSOLEBTN_ID:
			MessageBox(hWnd, TEXT("WM_COMMAND"), TEXT("WM_COMMAND title"), 0);
			std::cout << "hWnd: " << hWnd << " | mw.getCreateConsoleBtn() returned: " << mw.getCreateConsoleBtn() << std::endl;
			break;
		case CLOSECONSOLEBTN_ID:
			MessageBox(hWnd, TEXT("close"), TEXT("WM_COMMAND"), 0);
			std::cout << "hWnd: " << hWnd << " | mw.getCloseConsoleBtn() returned: " << mw.getCloseConsoleBtn() << std::endl;
			break;
		case COMBOBOX_ID:
			std::cout << "hWnd: " << hWnd << " | mw.getComboBox() returned: " << mw.getComboBox() << std::endl;
			switch (HIWORD(wParam)) {
			case CBN_DROPDOWN:
				std::cout << "CBN_DROPDOWN" << std::endl;
				break;
			case CBN_CLOSEUP:
				std::cout << "CBN_CLOSEUP" << std::endl;
				break;
			case CBN_SELENDCANCEL:
				std::cout << "CBN_SELENDCANCEL" << std::endl;
				break;
			case CBN_SELENDOK:
				std::cout << "CBN_SELENDOK" << std::endl;
				break;
			case CBN_SETFOCUS:
				std::cout << "CBN_SETFOCUS" << std::endl;
				break;
			case CBN_KILLFOCUS:
				std::cout << "CBN_KILLFOCUS" << std::endl;
				break;
			case CBN_SELCHANGE:
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, 
					(WPARAM)0, (LPARAM)0);
				TCHAR ListItem[256];
				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
				std::wstringstream oss;
				oss << ListItem << std::endl;
				//MessageBox(hWnd, (LPCWSTR)ListItem, L"Item selected", MB_OK);
				std::cout << "ItemSelected: \"" << utf8_encode(ListItem) << "\"" << std::endl;
				break;
			}
			break;
		}
		break;
	case WM_DRAWITEM:
		std::cout << "hWnd: " << hWnd << " | WM_DRAWITEM" << std::endl;
		break;
	case WM_DESTROY:
		if (init) {
			fclose(stdin);
			fclose(stdout);
			fclose(stderr);
			FreeConsole();
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MainWindow::onWmCreate(HWND hWnd) {
	OutputDebugString(TEXT(__FUNCTION__ "\n"));
	std::cout << "hWndMain: " << hWndMain << " | hWnd: " << hWnd << std::endl;
	std::cout << "tmpstr = \"" << tempstr << "\"" << std::endl;

	createConsoleBtn = CreateWindow(L"BUTTON", L"createButton", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 100,
		hWnd, (HMENU)CREATECONSOLEBTN_ID,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
	closeConsoleBtn = CreateWindow(L"BUTTON", L"closeButton", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		120, 10, 100, 100,
		hWnd, (HMENU)CLOSECONSOLEBTN_ID,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

	comboBox = CreateWindow(L"COMBOBOX", NULL, CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
		230, 10, 100, 100,
		hWnd, (HMENU)COMBOBOX_ID,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

	TCHAR NamesList[2][50] = {L"First value", L"Second value"};

	for (int index = 0; index < 2; index++) {
		SendMessage(comboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)NamesList[index]);
	}
	SendMessage(comboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	int Data = 0;
	HANDLE th1 = CreateThread(NULL, 0, threadMainFunc, &Data, 0, NULL);
	if (th1 != NULL) {
		//WaitForMultipleObjects(1, &th1, TRUE, INFINITE);
		WaitForSingleObject(th1, INFINITE);
		CloseHandle(th1);
	}

	std::string s;
	std::cout << "This works" << std::endl;
	std::cin >> s;
	std::cout << s << std::endl;
}

void DisplayMessage(HANDLE hScreen, LPCTSTR ThreadName, int Data, int Count) {
	const int buffSize = 255;
	TCHAR msgBuf[buffSize];
	size_t cchStringSize;
	DWORD dwChars;

	StringCchPrintf(msgBuf, buffSize, TEXT("Executing iteration %02d of %s, having data = %02d\n"), Count, ThreadName, Data);
	if (StringCchLength(msgBuf, buffSize, &cchStringSize) == S_OK)
		WriteConsole(hScreen, msgBuf, cchStringSize, &dwChars, NULL);
	Sleep(100);
}

DWORD WINAPI MainWindow::threadMainFunc(LPVOID lpParam) {
	int Data = 0;
	int count = 0;
	HANDLE hStdout = NULL;

	if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return 1;

	Data = *((int*)lpParam);

	for (count = 0; count <= 10; count++) {
		DisplayMessage(hStdout, TEXT("Thread"), Data, count);
	}
}

HWND MainWindow::getHWndMain() const {
	return hWndMain;
}

HWND MainWindow::getCreateConsoleBtn() const {
	return createConsoleBtn;
}

HWND MainWindow::getCloseConsoleBtn() const {
	return closeConsoleBtn;
}

HWND MainWindow::getComboBox() const {
	return comboBox;
}

std::string MainWindow::utf8_encode(const std::wstring& wstr) {
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), str.data(), size_needed, NULL, NULL);
	return str;
}

std::wstring MainWindow::utf8_decode(const std::string& str) {
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), wstr.data(), size_needed);
	return wstr;
}
