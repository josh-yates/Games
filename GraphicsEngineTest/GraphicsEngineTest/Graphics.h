#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Windows.h>
#include <stdexcept>
#include <memory>
#include <vector>
#include <d2d1.h>
#include <dwrite.h>
#include <functional>
#include <string>

//LINEAR OR SOLID: Make separate "SetSolidBrush" and "SetLinearBrush" functions that can then be used to edit brushes
//Also use flags at function input specifying whether to use linear or solid brush - could use enum or #define's

//TODO List:
//-Smart pointers
//-Either add bools for IsDrawingStarted(set when begin draw called and unset when end draw called) or move begin and end draw into shape drawing functions

namespace Graphics {
	struct Colour {
		Colour(const double Rin, const double Gin, const double Bin, const double Ain);
		double R;
		double G;
		double B;
		double A;
	};

	const enum BrushFlag { UseSolidBrush, UseLinearBrush };	//Tell shape drawing functions what brush to use

	class GraphicsEngine {
	private:
		ID2D1Factory* Factory;					//Generates graphics resources
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
		GraphicsEngine(HWND hWindow);
		~GraphicsEngine();
		void BeginDraw();						//Signal start of graphics drawing
		void EndDraw();							//Signal end of graphics drawing
		void SetSolidBrush(const double R, const double G, const double B, const double A);
		void SetLinearBrush(const std::vector<Graphics::Colour> GradientStops, const double StartX, const double StartY, const double EndX, const double EndY);
		void ClearScreen(const double R, const double G, const double B);
		void DrawEmptyCircle(const double X, const double Y, const double Radius, const double Thickness, const BrushFlag BrushToUse);
		void DrawFullCircle(const double X, const double Y, const double Radius, const BrushFlag BrushToUse);
		void DrawEmptyEllipse(const double X, const double Y, const double RadiusA, const double RadiusB, const double Thickness, const BrushFlag BrushToUse);
		void DrawFullEllipse(const double X, const double Y, const double RadiusA, const double RadiusB, const BrushFlag BrushToUse);
		void DrawEmptyRectangle(const double X, const double Y, const double Width, const double Height, const double Thickness, const BrushFlag BrushToUse);
		void DrawFullRectangle(const double X, const double Y, const double Width, const double Height, const BrushFlag BrushToUse);
		void DrawEmptySquare(const double X, const double Y, const double SideLength, const double Thickness, const BrushFlag BrushToUse);
		void DrawFullSquare(const double X, const double Y, const double SideLength, const BrushFlag BrushToUse);
		void WriteText(const std::string TextIn, const std::string FontName, const double FontSize, const double X, const double Y, const double Width, const double Height, const BrushFlag BrushToUse);

	private:
		//In shape drawing functions, only have to specify function for drawing the shape
		template<typename Lambda> void DrawShapeUsingBrush(BrushFlag BrushToUse, Lambda&& DrawingFunction) {
			switch (BrushToUse) {
			case UseSolidBrush:
				BrushSelection = SolidBrush;
				break;
			case UseLinearBrush:
				//if brush is nullptr, throw an error
				if (!LinearBrush) {
					throw std::invalid_argument("GraphicsEngine: Linear brush not set before use");
				}
				BrushSelection = LinearBrush;
				break;
			default:
				throw std::invalid_argument("GraphicsEngine: Invalid brush flag selected");
				break;
			}

			DrawingFunction();

			//Remove access of brush selection to the brush it pointed to
			BrushSelection = nullptr;
			//if any repeatedly created brushes were used (ie linear and radial), delete them
			//Solid brush is created once and repeatedly used so do not release it here
			if (LinearBrush) {
				LinearBrush->Release();
				LinearBrush = nullptr;
			}
		}
	};
}

#endif