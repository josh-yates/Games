//TESTING OF 2D GRAPHICS ENGINE
//Uses "What's a creel?" video tutorial on direct2d graphics

#include <Windows.h>
#include <d2d1.h>
#include <stdexcept>
#include <string>

#include "Graphics.h"

//GLOBAL VARIABLES
//Graphics engine
GraphicsEngine* Graphics;
RECT UpdatedRect;

//Window Values
HWND hMainWindow;
int ClientWidth{ 1000 };
int ClientHeight{ 700 };
int WindowXPos{ 100 };
int WindowYPos{ 100 };
LRESULT __stdcall WindowProc(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP);

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	try {
		//Register game window
		WNDCLASSW MainWindowClass{ 0 };
		MainWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		MainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		MainWindowClass.hInstance = hInst;
		MainWindowClass.lpszClassName = L"MainWindowClass";
		MainWindowClass.lpfnWndProc = WindowProc;

		if (!RegisterClassW(&MainWindowClass)) {
			throw std::invalid_argument("Failed to register MainWindowClass");
		}

		RECT MainWindowRect = { 0, 0, ClientWidth, ClientHeight };
		AdjustWindowRect(&MainWindowRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, false);

		//Show main window
		hMainWindow = CreateWindowW(L"MainWindowClass", L"Test", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
			WindowXPos, WindowYPos, MainWindowRect.right - MainWindowRect.left,
			MainWindowRect.bottom - MainWindowRect.top, NULL, NULL, NULL, NULL);

		//Initialise graphics engine
		Graphics = new GraphicsEngine();
		if (!Graphics->Init(hMainWindow)) {
			delete Graphics;
			throw std::invalid_argument("Unable to initialise graphics");
		}

		//Message Loop
		MSG Message{ 0 };
		while (Message.message != WM_QUIT) {
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else {
				//Update and render
				Graphics->BeginDraw();
				Graphics->ClearScreen(0.1, 0.1, 0.1);
				Graphics->DrawEmptyCircle(50, 50, 20, 0.0, 0.6, 0.6, 1.0, 4);
				Graphics->DrawFullCircle(150, 50, 20, 0.3, 0.2, 0.7, 1.0);
				Graphics->DrawEmptyEllipse(250, 50, 40, 20, 0.5, 0.2, 0.5, 1.0, 2);
				Graphics->DrawFullEllipse(350, 50, 50, 20, 0.9, 0.9, 0.2, 1.0);
				Graphics->DrawEmptySquare(450, 30, 40, 0.4, 0.3, 0.2, 1.0, 5);
				Graphics->DrawFullSquare(550, 30, 40, 0.8, 0.7, 0.3, 1.0);
				Graphics->DrawEmptyRectangle(650, 30, 50, 20, 0.3, 0.3, 0.5, 1.0, 3);
				Graphics->DrawFullRectangle(750, 30, 100, 50, 0.9, 0.2, 0.2, 1.0);
				Graphics->EndDraw();
			}
		}
		//Clean up memory
		delete Graphics;

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

LRESULT __stdcall WindowProc(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP) {
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