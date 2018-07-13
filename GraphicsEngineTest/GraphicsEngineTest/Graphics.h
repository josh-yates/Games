#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Windows.h>
#include <stdexcept>
#include <vector>
#include <d2d1.h>
#include <dwrite.h>
#include <string>

//LINEAR OR SOLID: Make separate "SetSolidBrush" and "SetLinearBrush" functions that can then be used to edit brushes
//Also use flags at function input specifying whether to use linear or solid brush - could use enum or #define's

namespace Graphics {
	struct Colour {
		Colour(const double Rin, const double Gin, const double Bin, const double Ain);
		double R;
		double G;
		double B;
		double A;
	};

	const enum BrushFlag { UseSolidBrush, UseLinearBrush };

	class GraphicsEngine {
	private:
		ID2D1Factory * Factory;					//Generates graphics resources
		IDWriteFactory* TextFactory;			//Generates text resources
		ID2D1HwndRenderTarget* RenderTarget;	//Where to render resources to
		ID2D1Brush* BrushSelection;				//Set to linear, solid or radial brush
		ID2D1SolidColorBrush* SolidBrush;		//Brush for drawing shapes with solid color
		ID2D1LinearGradientBrush* LinearBrush;	//Brush for drawing shapes with gradient color
		D2D1_GRADIENT_STOP* GradientStopsArray;	//Pointer for dynamic allocation of gradient stop array
		ID2D1GradientStopCollection* GradientStopCollection;
		IDWriteTextFormat* TextFormat;			//Formatter for text
		HRESULT EventResult;
		std::wstring StringToWstring(const std::string StringIn);	//String to wstring converter
	public:
		//TODO add flags for whether to use solid brush or linear brush and remove colour inputs, ie. you set the brush then draw a shape and use brush flag to specify which brush to use
		//TODO also add a starting linear gradient brush in init. Potentially move init to constructor for safety
		GraphicsEngine();
		~GraphicsEngine();
		bool Init(HWND hWindow);				//Initialise direct2d
		void BeginDraw();						//Signal start of graphics drawing
		void EndDraw();							//Signal end of graphics drawing
		void SetSolidBrush(double R, double G, double B, double A);
		void SetLinearBrush(const std::vector<Graphics::Colour> GradientStops, const double StartX, const double StartY, const double EndX, const double EndY);
		void ClearScreen(double R, double G, double B);
		void DrawEmptyCircle(const double X, const double Y, const double Radius, const double Thickness, const BrushFlag BrushToUse);
		void DrawFullCircle(double X, double Y, double Radius, double R, double G, double B, double A);
		void DrawEmptyEllipse(double X, double Y, double RadiusA, double RadiusB, double R, double G, double B, double A, double Thickness);
		void DrawFullEllipse(double X, double Y, double RadiusA, double RadiusB, double R, double G, double B, double A);
		void DrawEmptyRectangle(double X, double Y, double Width, double Height, double R, double G, double B, double A, double Thickness);
		void DrawFullRectangle(double X, double Y, double Width, double Height, double R, double G, double B, double A);
		void DrawEmptySquare(double X, double Y, double SideLength, double R, double G, double B, double A, double Thickness);
		void DrawFullSquare(double X, double Y, double SideLength, double R, double G, double B, double A);
		void WriteText(std::string TextIn, std::string FontName, double FontSize, double X, double Y, double Width, double Height, double R, double G, double B, double A);
	};
}

#endif