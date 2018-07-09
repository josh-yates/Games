#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Windows.h>
#include <d2d1.h>
#include <string>

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
	void DrawEmptyCircle(double X, double Y, double Radius, double R, double G, double B, double A, double Thickness);
	void DrawFullCircle(double X, double Y, double Radius, double R, double G, double B, double A);
	void DrawEmptyEllipse(double X, double Y, double RadiusA, double RadiusB, double R, double G, double B, double A, double Thickness);
	void DrawFullEllipse(double X, double Y, double RadiusA, double RadiusB, double R, double G, double B, double A);
	void DrawEmptyRectangle(double X, double Y, double Width, double Height, double R, double G, double B, double A, double Thickness);
	void DrawFullRectangle(double X, double Y, double Width, double Height, double R, double G, double B, double A);
	void DrawEmptySquare(double X, double Y, double SideLength, double R, double G, double B, double A, double Thickness);
	void DrawFullSquare(double X, double Y, double SideLength, double R, double G, double B, double A);
	//TODO add extra functions for ellipse, square etc and finish them. Rename drawcircle to DrawEmptyCircle etc to avoid conflicts
};

#endif