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

//Ball variables
double ballYPos{ 0.0 };
double ballYSpeed{ 0.0 };
double ballRadius{ 25.0 };

//Game Window
HWND hGameWindow;
int GameClientWidth{ 700 };
int GameClientHeight{ 700 };
int GameWindowXPos{ 100 };
int GameWindowYPos{ 100 };
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

		RECT GameWindowRect = { 0, 0, GameClientWidth, GameClientHeight };
		AdjustWindowRect(&GameWindowRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, false);

		//Show game window
		hGameWindow = CreateWindowW(L"GameWindowClass", L"Breakout", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
			GameWindowXPos, GameWindowYPos, GameWindowRect.right - GameWindowRect.left,
			GameWindowRect.bottom - GameWindowRect.top, NULL, NULL, NULL, NULL);

		//Initialise graphics engine
		Graphics = new GraphicsEngine();
		if (!Graphics->Init(hGameWindow)) {
			delete Graphics;
			throw std::invalid_argument("Unable to initialise graphics");
		}

		//Message Loop
		MSG Message{ 0 };
		while (Message.message!=WM_QUIT) {
			if (PeekMessage(&Message, NULL, 0, 0,PM_REMOVE)) {
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else {
				//Update and render
				ballYSpeed += 0.1;									//TODO Continue this in separate game physics engine
				ballYPos += ballYSpeed;
				if (ballYPos > GameClientHeight - ballRadius) {
					ballYPos = GameClientHeight - ballRadius;
					ballYSpeed = -5.0;
				}
				Graphics->BeginDraw();
				Graphics->ClearScreen(0.0, 0.3, 0.7);
				Graphics->DrawCircle(100, ballYPos, ballRadius, 1.0, 0.0, 1.0, 1.0);
				Graphics->EndDraw();
			}
		}

		//while (GetMessage(&Message, NULL, NULL, NULL)) {		//Original message loop
		//	TranslateMessage(&Message);
		//	DispatchMessage(&Message);
		//}

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

LRESULT __stdcall GameWindowProc(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP) {
	switch (Message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	//case WM_PAINT:
	//	Graphics->BeginDraw();
	//	Graphics->ClearScreen(1, 1, 1);
	//	//create 50 random circles
	//	for (int i{ 0 }; i < 50; i++) {
	//		Graphics->DrawCircle(rand() % GameWindowWidth, rand() % GameWindowHeight, rand() % 100, (rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	//	}
	//	Graphics->EndDraw();
	//	GetClientRect(hGameWindow, &UpdatedRect);
	//	ValidateRect(hGameWindow, &UpdatedRect);
	//	break;
	default:
		return DefWindowProcW(hWindow, Message, wP, lP);
		break;
	}
}