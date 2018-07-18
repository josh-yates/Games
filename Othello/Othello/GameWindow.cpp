#include "GameWindow.h"

HWND GameWindow::hGameWindow;

void GameWindow::InitialiseGameWindow(HINSTANCE hInst, const int xPosIn, const int yPosIn, const int ClientWidthIn, const int ClientHeightIn) {
	WNDCLASSW GameWindowClass{ 0 };
	GameWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	GameWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	GameWindowClass.hInstance = hInst;
	GameWindowClass.lpszClassName = L"GameWindowClass";
	GameWindowClass.lpfnWndProc = GameWindowProcedure;

	if (!RegisterClassW(&GameWindowClass)) {
		throw std::runtime_error("Failed to register MainWindowClass");
	}

	//Resize window about client dimensions
	RECT GameWindowRect = { 0,0,ClientWidthIn,ClientHeightIn };
	DWORD GameWindowStyle{ WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE };
	AdjustWindowRect(&GameWindowRect, GameWindowStyle, false);
	hGameWindow = CreateWindowW(L"GameWindowClass", L"Othello", GameWindowStyle, xPosIn, yPosIn, GameWindowRect.right - GameWindowRect.left, GameWindowRect.bottom - GameWindowRect.top, nullptr, nullptr, nullptr, nullptr);
}

void GameWindow::DrawMenuScreen(Graphics::GraphicsEngine* DrawingEngine, const int ClientWidthIn, const int ClientHeightIn) {
	//TODO do the placement maths for this so if window size is changed it will scale automatically
	DrawingEngine->BeginDraw();
	DrawingEngine->ClearScreen(0.0, 0.0, 0.0);
	//Draw background
	std::vector<Graphics::Colour> MenuGradient{ Graphics::Colour(0.0,0.15,0.0,1.0),Graphics::Colour(0.0,0.39,0.0,1.0) };
	DrawingEngine->SetLinearBrush(MenuGradient, 0, 0, ClientWidthIn, ClientHeightIn);
	DrawingEngine->DrawFullRectangle(0, 0, ClientWidthIn, ClientHeightIn, Graphics::UseLinearBrush);
	//Draw title
	DrawingEngine->SetSolidBrush(1.0, 1.0, 1.0, 1.0);
	DrawingEngine->WriteText("Othello", "Times New Roman", 80, ((ClientWidthIn - 300) / 2) + 30, 50, 300, 100, Graphics::UseSolidBrush);
	//Draw player 1 option tiles
	DrawingEngine->WriteText("Player 1", "Times New Roman", 40, 100, 200, 300, 80, Graphics::UseSolidBrush);
	std::vector<Graphics::Colour> WhiteTileGradient{ Graphics::Colour(0.5,0.5,0.5,0.8),Graphics::Colour(1.0,1.0,1.0,0.8) };
	DrawingEngine->SetLinearBrush(WhiteTileGradient, 320, 170, 420, 270);
	DrawingEngine->DrawFullCircle(370, 220, 50, Graphics::UseLinearBrush);
	std::vector<Graphics::Colour> BlackTileGradient{ Graphics::Colour(0.0,0.0,0.0,0.8),Graphics::Colour(0.2,0.2,0.2,0.8) };
	DrawingEngine->SetLinearBrush(BlackTileGradient, 450, 170, 550, 270);
	DrawingEngine->DrawFullCircle(500, 220, 50, Graphics::UseLinearBrush);
	DrawingEngine->SetSolidBrush(0.0, 0.0, 0.0, 1.0);
	DrawingEngine->WriteText("Human", "Times new roman", 30, 325, 200, 100, 100, Graphics::UseSolidBrush);
	DrawingEngine->SetSolidBrush(1.0, 1.0, 1.0, 1.0);
	DrawingEngine->WriteText("CPU", "Times new roman", 30, 470, 200, 100, 100, Graphics::UseSolidBrush);
	//Draw player 2 option tiles
	DrawingEngine->WriteText("Player 2", "Times New Roman", 40, 100, 320, 300, 80, Graphics::UseSolidBrush);
	DrawingEngine->SetLinearBrush(WhiteTileGradient, 320, 290, 420, 390);
	DrawingEngine->DrawFullCircle(370, 340, 50, Graphics::UseLinearBrush);
	DrawingEngine->SetLinearBrush(BlackTileGradient, 450, 290, 550, 390);
	DrawingEngine->DrawFullCircle(500, 340, 50, Graphics::UseLinearBrush);
	DrawingEngine->SetSolidBrush(0.0, 0.0, 0.0, 1.0);
	DrawingEngine->WriteText("Human", "Times new roman", 30, 325, 320, 100, 100, Graphics::UseSolidBrush);
	DrawingEngine->SetSolidBrush(1.0, 1.0, 1.0, 1.0);
	DrawingEngine->WriteText("CPU", "Times new roman", 30, 470, 320, 100, 100, Graphics::UseSolidBrush);
	DrawingEngine->EndDraw();
}