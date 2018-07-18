//Othello/reversi game

#include <Windows.h>
#include <stdexcept>
#include <string>
#include <Memory>

#include "Graphics.h"
#include "GameWindow.h"

std::wstring StringToWstring(const std::string StringIn) {
	std::wstring Converted(StringIn.length(), L' ');
	std::copy(StringIn.begin(), StringIn.end(), Converted.begin());
	return Converted;
}

//GLOBAL VARIABLES
std::unique_ptr<Graphics::GraphicsEngine> DrawingEngine;
const int ClientWidth{ 600 };
const int ClientHeight{ 600 };

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	try {
		GameWindow::InitialiseGameWindow(hPrevInst, 100, 100, ClientWidth, ClientHeight);
		DrawingEngine = std::make_unique<Graphics::GraphicsEngine>(GameWindow::hGameWindow);

		MSG Message{ 0 };
		while (GetMessage(&Message, NULL, NULL, NULL)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	catch (std::invalid_argument& InvalArg) {
		MessageBox(NULL, (StringToWstring(InvalArg.what())).c_str(), L"Programming error", MB_ICONERROR);
	}
	catch (std::runtime_error& RunError) {
		MessageBox(NULL, (StringToWstring(RunError.what())).c_str(), L"Runtime error", MB_ICONERROR);
	}
	catch (...) {
		MessageBox(NULL, L"Unknown error", L"Programming error", MB_ICONERROR);
	}
	return 0;
}

namespace GameWindow {
	LRESULT CALLBACK GameWindowProcedure(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP) {
		try {
			switch (Message) {
			case WM_PAINT:
				//Paint window
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWindow, &ps);
				EndPaint(hWindow, &ps);
				DrawMenuScreen(DrawingEngine.get(), ClientWidth, ClientHeight);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
				break;
			default:
				return DefWindowProcW(hWindow, Message, wP, lP);
				break;
			}
		}
		catch (std::invalid_argument& InvalArg) {
			MessageBox(NULL, (StringToWstring(InvalArg.what())).c_str(), L"Programming error", MB_ICONERROR);
		}
		catch (std::runtime_error& RunError) {
			MessageBox(NULL, (StringToWstring(RunError.what())).c_str(), L"Runtime error", MB_ICONERROR);
		}
		catch (...) {
			MessageBox(NULL, L"Unknown error", L"Programming error", MB_ICONERROR);
		}
	}
}