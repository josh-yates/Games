//TESTING OF 2D GRAPHICS ENGINE
//Uses "What's a creel?" video tutorial on direct2d graphics

#include <Windows.h>
#include <d2d1.h>
#include <stdexcept>
#include <string>

#include "Graphics.h"

//GLOBAL VARIABLES
//Graphics engine
Graphics::GraphicsEngine* Renderer;
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
		Renderer = new Graphics::GraphicsEngine(hMainWindow);

		//Message Loop
		MSG Message{ 0 };
		while (Message.message != WM_QUIT) {
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else {
				//Update and render
				Renderer->BeginDraw();
				Renderer->ClearScreen(0.1, 0.2, 0.1);
				std::vector<Graphics::Colour> MyStops{ Graphics::Colour(1.0, 1.0, 1.0, 1.0), Graphics::Colour(1.0, 1.0, 1.0, 0.0) };
				Renderer->SetLinearBrush(MyStops, 80, 50, 110, 50);
				Renderer->DrawEmptyCircle(100, 50, 20, 4, Graphics::UseLinearBrush);
				Renderer->SetLinearBrush(MyStops, 120, 20, 180, 80);
				Renderer->DrawFullCircle(150, 50, 30, Graphics::UseLinearBrush);
				MyStops = { Graphics::Colour(0.0, 1.0, 0.0, 1.0), Graphics::Colour(0.0, 0.0, 1.0, 1.0) };
				Renderer->SetLinearBrush(MyStops, 250, 30, 250, 70);
				Renderer->DrawEmptyEllipse(250, 50, 40, 20, 6, Graphics::UseLinearBrush);
				MyStops = { Graphics::Colour(0.0, 1.0, 0.0, 1.0), Graphics::Colour(0.0, 0.0, 1.0, 1.0), Graphics::Colour(1.0,0.0,0.0,1.0) };
				Renderer->SetLinearBrush(MyStops, 350, 30, 350, 70);
				Renderer->DrawFullEllipse(350, 50, 50, 20, Graphics::UseLinearBrush);
				Renderer->SetLinearBrush(MyStops, 450, 50, 490, 50);
				Renderer->DrawEmptySquare(450, 30, 40, 5, Graphics::UseLinearBrush);
				MyStops = { Graphics::Colour(0.5, 0.0, 1.0, 1.0), Graphics::Colour(1.0,1.0,1.0,1.0) };
				Renderer->SetLinearBrush(MyStops, 550, 70, 590, 30);
				Renderer->DrawFullSquare(550, 30, 40, Graphics::UseLinearBrush);
				MyStops = { Graphics::Colour(0.0, 1.0, 0.0, 1.0), Graphics::Colour(0.0, 1.0, 0.0, 0.0) };
				Renderer->SetLinearBrush(MyStops, 650, 30, 700, 50);
				Renderer->DrawEmptyRectangle(650, 30, 50, 20, 4, Graphics::UseLinearBrush);
				Renderer->SetLinearBrush(MyStops, 850, 30, 750, 80);
				Renderer->DrawFullRectangle(750, 30, 100, 50, Graphics::UseLinearBrush);
				MyStops = { Graphics::Colour(1.0,0.0,0.0,1.0),Graphics::Colour(0.0,0.0,1.0,1.0) };
				Renderer->SetLinearBrush(MyStops, 10, 120, 1000, 120);
				Renderer->WriteText("abcdefghijklmnopqrstuvwxyz0123456789#'/.,\@:;(){}[]", "comic sans ms", 30, 10, 300, 1000, 200, Graphics::UseLinearBrush);
				Renderer->EndDraw();
			}
		}
		//Clean up memory
		delete Renderer;

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