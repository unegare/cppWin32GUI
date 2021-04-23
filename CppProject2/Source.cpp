#include <windows.h>
#include "MainWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	try {
		mw = std::move(MainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow));
		std::ostringstream ss;
		ss << __FUNCTION__ << ": " << mw.getHWndMain() << std::endl;
		ss << __FUNCTION__ << ": &mw == " << &mw << std::endl;
		OutputDebugStringA(ss.str().c_str());
		OutputDebugStringA(ss.str().c_str());
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	MSG msg = { 0 };
	int iGetOk = 0;
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (iGetOk == -1) return 3;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}