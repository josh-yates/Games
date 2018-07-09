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
std::vector<Ball> Balls;
std::vector<std::vector<double>> BallColours;

//Game Window
HWND hGameWindow;
int GameClientWidth{ 1000 };
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
		for (int i{ 0 }; i < 100; i++) {
			Vector2D tempVelocity(rand() % 71 + (-35), rand() % 51 + (-25));
			Ball tempBall(1, rand() % GameClientWidth, rand() % GameClientHeight, (rand() % 20) + 5, tempVelocity);
			Balls.push_back(tempBall);
			BallColours.push_back({ (rand() % 100) / 100.0,(rand() % 100) / 100.0 ,(rand() % 100) / 100.0 ,(rand() % 100) / 100.0 });
		}
		for (auto iter{ Balls.begin() }; iter != Balls.end(); iter++) {
			Physics->AddObject(&*iter);
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
				Physics->Update(0.15);
				Graphics->BeginDraw();
				Graphics->ClearScreen(0.0, 0.3, 0.7);
				for (size_t i{ 0 }; i < Balls.size(); i++) {
					Graphics->DrawEmptyCircle(Balls[i].getXPos(), Balls[i].getYPos(), Balls[i].getRadius(), BallColours[i].at(0), BallColours[i].at(1), BallColours[i].at(2), BallColours[i].at(3),5);
				}
				Graphics->EndDraw();
			}
		}
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
	default:
		return DefWindowProcW(hWindow, Message, wP, lP);
		break;
	}
}