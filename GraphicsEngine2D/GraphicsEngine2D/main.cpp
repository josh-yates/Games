//TESTING OF 2D GRAPHICS ENGINE
//Uses "What's a creel?" video tutorial on direct2d graphics

#include <Windows.h>
#include <d2d1.h>
#include <stdexcept>
#include <string>

#include "Graphics.h"
#include "Physics.h"

//GLOBAL VARIABLES
//Graphics engine
GraphicsEngine* Graphics;
PhysicsEngine* Physics;
RECT UpdatedRect;

//Physics engine test
Vector2D Gravity(0, 9.81);
Vector2D StartVelocity(-40, -10);
Ball MyFirstBall(10, 200, 100, 20, StartVelocity);
std::vector<Ball> Balls;
std::vector<std::vector<double>> BallPath;
int PathCount{ 1 };

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

		//Initialise physics engine and add test balls
		Physics = new PhysicsEngine(GameClientWidth, 0, GameClientHeight, 0, Gravity);
		Physics->AddObject(&MyFirstBall);

		//Message Loop
		MSG Message{ 0 };
		while (Message.message!=WM_QUIT) {
			if (PeekMessage(&Message, NULL, 0, 0,PM_REMOVE)) {
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else {
				//Update and render
				Physics->Update(0.15);
				Graphics->BeginDraw();
				Graphics->ClearScreen(0.0, 0.3, 0.7);
				if (PathCount == 6) {
					BallPath.push_back({ MyFirstBall.getXPos(), MyFirstBall.getYPos() });
					PathCount = 1;
				}
				else {
					PathCount += 1;
				}
				for (auto iter{ BallPath.begin() }; iter != BallPath.end(); iter++) {
					Graphics->DrawCircle(iter->at(0), iter->at(1), 5, 1.0, 0.0, 0.0, 0.8);
				}
				Graphics->DrawCircle(MyFirstBall.getXPos(), MyFirstBall.getYPos(), MyFirstBall.getRadius(), 0.5, 0.8, 0.2, 1.0);
				Graphics->EndDraw();
			}
		}

		//while (GetMessage(&Message, NULL, NULL, NULL)) {		//Original message loop
		//	TranslateMessage(&Message);
		//	DispatchMessage(&Message);
		//}

		//Clean up memory
		delete Graphics;
		delete Physics;

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