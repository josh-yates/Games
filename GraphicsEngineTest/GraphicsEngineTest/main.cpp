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
		while (GetMessage(&Message, NULL, NULL, NULL)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
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
	try {
		switch (Message) {
		case WM_PAINT: {
			//Paint window
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWindow, &ps);
			EndPaint(hWindow, &ps);
			//Paint app
			Renderer->BeginDraw();
			Renderer->ClearScreen(0.0, 0.0, 0.0);
			std::vector<Graphics::Colour> Stops{ Graphics::Colour(0.62,0.42,0.52,1.0),Graphics::Colour(0.95,0.95,0.95,1.0) };
			Renderer->SetLinearBrush(Stops, ClientWidth / 2, 0, ClientWidth / 2, ClientHeight);
			Renderer->DrawFullRectangle(0, 0, ClientWidth, ClientHeight, Graphics::UseLinearBrush);
			Renderer->SetSolidBrush(0.0, 0.0, 0.0, 1.0);
			Renderer->WriteText("Example Program", "Gill sans nova", 70, 250, 50, 600, 100, Graphics::UseSolidBrush);
			Stops = { Graphics::Colour(0.5,0.5,0.5,1.0),Graphics::Colour(0.7,0.7,0.7,1.0), Graphics::Colour(0.5,0.5,0.5,1.0) };
			Renderer->SetLinearBrush(Stops, (ClientWidth - 300) / 2, 400, ((ClientWidth - 300) / 2) + 300, 400);
			Renderer->DrawFullRectangle((ClientWidth - 300) / 2, 400, 300, 80, Graphics::UseLinearBrush);
			Renderer->SetSolidBrush(0.0, 0.0, 0.0, 0.5);
			Renderer->DrawEmptyRectangle((ClientWidth - 300) / 2, 400, 300, 80, 4, Graphics::UseSolidBrush);
			Renderer->SetSolidBrush(0.0, 0.0, 0.0, 1.0);
			Renderer->WriteText("Example Button", "Arial Black", 25, ((ClientWidth - 300) / 2) + 55, 420, 200, 30, Graphics::UseSolidBrush);
			//Draw background
			Renderer->EndDraw();
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		default:
			return DefWindowProcW(hWindow, Message, wP, lP);
			break;
		}
	}
	catch (std::invalid_argument& inval_arg) {
		//copy string to wstring
		std::string TempString(inval_arg.what());
		std::wstring errmsg(TempString.length(), L' ');
		std::copy(TempString.begin(), TempString.end(), errmsg.begin());
		MessageBox(NULL, errmsg.c_str(), L"Programming error", MB_ICONERROR);
		PostQuitMessage(0);
		return 0;
	}
	catch (...) {
		MessageBox(NULL, L"Unknown error", L"Programming error", MB_ICONERROR);
		PostQuitMessage(0);
		return 0;
	}
}