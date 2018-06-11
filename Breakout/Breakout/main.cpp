//BREAKOUT GAME PROGRAMMING PROJECT USING DIRECT2D

#include <Windows.h>
#include <d2d1_1.h>
#include <stdexcept>
#include <string>

HWND hGameWindow;
LRESULT __stdcall GameWindowProc(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP);

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	try {
		//Register game window
		WNDCLASSW GameWindowClass{ 0 };
		GameWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		GameWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		GameWindowClass.hInstance = hInst;
		GameWindowClass.lpszClassName = L"GameWindowClass";
		GameWindowClass.lpfnWndProc = GameWindowProc;

		if (!RegisterClassW(&GameWindowClass)) {
			throw std::invalid_argument("Failed to register GameWindowClass");
		}

		//Show game window
		hGameWindow = CreateWindowW(L"GameWindowClass", L"Breakout", WS_SYSMENU | WS_VISIBLE,
			100, 100, 300, 300, NULL, NULL, NULL, NULL);

		//Message Loop
		MSG Message{ 0 };
		while (GetMessage(&Message, NULL, NULL, NULL)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	catch (std::invalid_argument& inval_arg) {
		//copy string to wstring
		std::string TempString(inval_arg.what());
		std::wstring errmsg(TempString.length(), L' ');
		std::copy(TempString.begin(), TempString.end(), errmsg.begin());
		MessageBox(NULL, errmsg.c_str(), L"Programming error", MB_ICONERROR);
	}
	catch (...) {
		MessageBox(NULL, L"Unknown error", L"Programming error", MB_ICONERROR);
	}
	return 0;
}

LRESULT __stdcall GameWindowProc(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP) {
	switch (Message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		return DefWindowProcW(hWindow, Message, wP, lP);
		break;
	}
}