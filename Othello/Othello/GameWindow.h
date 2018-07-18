#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <Windows.h>
#include <stdexcept>
#include "Graphics.h"

namespace GameWindow {
	extern HWND hGameWindow;
	LRESULT CALLBACK GameWindowProcedure(HWND hWindow, UINT Message, WPARAM wP, LPARAM lP);
	void InitialiseGameWindow(HINSTANCE hInst, const int xPosIn, const int yPosIn, const int ClientWidthIn, const int ClientHeightIn);
	void DrawMenuScreen(Graphics::GraphicsEngine* DrawingEngine, const int ClientWidthIn, const int ClientHeightIn);
}
#endif