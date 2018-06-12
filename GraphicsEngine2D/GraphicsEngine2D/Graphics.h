#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <Windows.h>
#include <d2d1.h>

class GraphicsEngine {
private:
	ID2D1Factory * Factory;					//Generates resources
	ID2D1HwndRenderTarget* RenderTarget;	//Where to render resources to
	ID2D1SolidColorBrush* Brush;		//Brush for drawing shapes
public:
	GraphicsEngine();
	~GraphicsEngine();
	bool Init(HWND hWindow);				//Initialise direct2d
	void BeginDraw();						//Signal start of graphics drawing
	void EndDraw();							//Signal end of graphics drawing
	void ClearScreen(double R, double G, double B);
	void DrawCircle(double X, double Y, double Radius, double R, double G, double B, double A);
};

#endif