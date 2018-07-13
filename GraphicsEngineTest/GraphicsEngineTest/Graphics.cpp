#include "Graphics.h"

namespace Graphics {
	Colour::Colour(const double Rin, const double Gin, const double Bin, const double Ain) :R(Rin), G(Gin), B(Bin), A(Ain) {}

	GraphicsEngine::GraphicsEngine(HWND hWindow) {
		Factory = nullptr;
		TextFactory = nullptr;
		RenderTarget = nullptr;
		BrushSelection = nullptr;
		SolidBrush = nullptr;
		LinearBrush = nullptr;
		GradientStopsArray = nullptr;
		GradientStopCollection = nullptr;
		TextFormat = nullptr;
		EventResult = S_OK;

		//Initialise resources

		//Try initialising the factory
		EventResult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
		if (EventResult != S_OK) {
			throw std::invalid_argument("GraphicsEngine constructor: Unable to create factory");
		}

		//Read the size of the window
		RECT WindowRect;
		GetClientRect(hWindow, &WindowRect);

		//Try initialising the rendertarget
		EventResult = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWindow, D2D1::SizeU(WindowRect.right, WindowRect.bottom)),
			&RenderTarget);

		if (EventResult != S_OK) {
			throw std::invalid_argument("GraphicsEngine constructor: Unable to initialise render target");
		}

		//Globally create the solid brush for repeated use
		EventResult = RenderTarget->CreateSolidColorBrush(D2D1::ColorF(static_cast<float>(0.0), static_cast<float>(0.0), static_cast<float>(0.0), static_cast<float>(1.0)), &SolidBrush);

		if (EventResult != S_OK) {
			throw std::invalid_argument("GraphicsEngine constructor: Unable to create solid brush");
		}

		//Create the text factory
		EventResult = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&TextFactory));

		if (EventResult != S_OK) {
			throw std::invalid_argument("GraphicsEngine constructor: Unable to create text factory");
		}
	}

	GraphicsEngine::~GraphicsEngine() {
		//Release resources
		if (Factory) {
			Factory->Release();
		}
		if (TextFactory) {
			TextFactory->Release();
		}
		if (RenderTarget) {
			RenderTarget->Release();
		}
		if (SolidBrush) {
			SolidBrush->Release();
		}
		if (LinearBrush) {
			LinearBrush->Release();
		}
		if (BrushSelection) {
			BrushSelection->Release();
		}
		if (GradientStopCollection) {
			GradientStopCollection->Release();
		}
		if (TextFormat) {
			TextFormat->Release();
		}
	}

	std::wstring GraphicsEngine::StringToWstring(const std::string StringIn) {
		std::wstring Converted(StringIn.length(), L' ');
		std::copy(StringIn.begin(), StringIn.end(), Converted.begin());
		return Converted;
	}
	void GraphicsEngine::BeginDraw() {
		RenderTarget->BeginDraw();
	}
	void GraphicsEngine::EndDraw() {
		RenderTarget->EndDraw();
	}
	void GraphicsEngine::ClearScreen(const double R, const double G, const double B) {
		RenderTarget->Clear(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B)));
	}

	void GraphicsEngine::SetSolidBrush(const double R, const double G, const double B, const double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
	}

	void GraphicsEngine::SetLinearBrush(const std::vector<Graphics::Colour> GradientStops, const double StartX, const double StartY, const double EndX, const double EndY) {
		if (GradientStops.size() == 0) {
			throw std::invalid_argument("GraphicsEngine::SetLinearBrush: Empty GradientStops vector entered");
		}
		//check if linear brush not cleared
		if (LinearBrush) {
			LinearBrush->Release();
			LinearBrush = nullptr;
		}
		//Potentially throws bad_alloc
		GradientStopsArray = new D2D1_GRADIENT_STOP[GradientStops.size()];
		for (int i{ 0 }; i < GradientStops.size(); i++) {
			GradientStopsArray[i].color = D2D1::ColorF(static_cast<float>(GradientStops[i].R), static_cast<float>(GradientStops[i].G), static_cast<float>(GradientStops[i].B), static_cast<float>(GradientStops[i].A));
			GradientStopsArray[i].position = static_cast<float>(static_cast<double>(i) / (static_cast<double>(GradientStops.size() - 1)));
		}
		EventResult = RenderTarget->CreateGradientStopCollection(GradientStopsArray, GradientStops.size(), &GradientStopCollection);
		delete[] GradientStopsArray;
		GradientStopsArray = nullptr;
		if (EventResult != S_OK) {
			throw std::invalid_argument("GraphicsEngine::SetLinearBrush: Gradient stop collection generation failure");
		}
		EventResult = RenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(static_cast<float>(StartX), static_cast<float>(StartY)), D2D1::Point2F(static_cast<float>(EndX), static_cast<float>(EndY))), GradientStopCollection, &LinearBrush);
		GradientStopCollection->Release();
		GradientStopCollection = nullptr;
	}

	void GraphicsEngine::DrawEmptyCircle(const double X, const double Y, const double Radius, const double Thickness, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, Radius, Thickness]()-> void {RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), BrushSelection, static_cast<float>(abs(Thickness))); });
	}

	void GraphicsEngine::DrawFullCircle(const double X, const double Y, const double Radius, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, Radius]()->void {RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), BrushSelection); });
	}

	void GraphicsEngine::DrawEmptyEllipse(const double X, const double Y, const double RadiusA, const double RadiusB, const double Thickness, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, RadiusA, RadiusB, Thickness]()->void {RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(RadiusA), static_cast<float>(RadiusB)), BrushSelection, static_cast<float>(abs(Thickness))); });
	}

	void GraphicsEngine::DrawFullEllipse(const double X, const double Y, const double RadiusA, const double RadiusB, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, RadiusA, RadiusB]()->void {RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(RadiusA), static_cast<float>(RadiusB)), BrushSelection); });
	}

	void GraphicsEngine::DrawEmptyRectangle(const double X, const double Y, const double Width, const double Height, const double Thickness, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, Width, Height, Thickness]()->void {RenderTarget->DrawRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + Width), static_cast<float>(Y + Height)), BrushSelection, static_cast<float>(abs(Thickness))); });
	}

	void GraphicsEngine::DrawFullRectangle(const double X, const double Y, const double Width, const double Height, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, Width, Height]()->void {RenderTarget->FillRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + Width), static_cast<float>(Y + Height)), BrushSelection); });
	}

	void GraphicsEngine::DrawEmptySquare(const double X, const double Y, const double SideLength, const double Thickness, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, SideLength, Thickness]()->void {RenderTarget->DrawRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + SideLength), static_cast<float>(Y + SideLength)), BrushSelection, static_cast<float>(abs(Thickness))); });
	}

	void GraphicsEngine::DrawFullSquare(const double X, const double Y, const double SideLength, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, X, Y, SideLength]()->void {RenderTarget->FillRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + SideLength), static_cast<float>(Y + SideLength)), BrushSelection); });
	}

	void GraphicsEngine::DrawStraightLine(const double StartX, const double StartY, const double EndX, const double EndY, const double Thickness, const BrushFlag BrushToUse) {
		DrawShapeUsingBrush(BrushToUse, [this, StartX, StartY, EndX, EndY, Thickness]()->void {RenderTarget->DrawLine(D2D1::Point2F(static_cast<float>(StartX), static_cast<float>(StartY)), D2D1::Point2F(static_cast<float>(EndX), static_cast<float>(EndY)), BrushSelection,static_cast<float>(Thickness)); });
	}

	void GraphicsEngine::WriteText(const std::string TextIn, const std::string FontName, const double FontSize, const double X, const double Y, const double Width, const double Height, const BrushFlag BrushToUse) {
		if (TextFormat) {
			TextFormat->Release();
			TextFormat = nullptr;
		}
		EventResult = TextFactory->CreateTextFormat((StringToWstring(FontName)).c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, static_cast<float>(abs(FontSize)), L"en-us", &TextFormat);
		if (EventResult != S_OK) {
			//Not thrown by invalid font name
			throw std::invalid_argument("GraphicsEngine::WriteText: Text format generation failure");
		}
		DrawShapeUsingBrush(BrushToUse, [this, TextIn, FontName, FontSize, X, Y, Width, Height]()->void {RenderTarget->DrawText((StringToWstring(TextIn)).c_str(), TextIn.size(), TextFormat, D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + Width), static_cast<float>(Y + Height)), BrushSelection, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); });
		TextFormat->Release();
		TextFormat = nullptr;
	}
}