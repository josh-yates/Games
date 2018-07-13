#include "Graphics.h"

namespace Graphics {
	Colour::Colour(const double Rin, const double Gin, const double Bin, const double Ain) :R(Rin), G(Gin), B(Bin), A(Ain) {}

	GraphicsEngine::GraphicsEngine() {
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

	bool GraphicsEngine::Init(HWND hWindow) {
		//Try initialising the factory
		EventResult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
		if (EventResult != S_OK) {
			return false;
		}

		//Read the size of the window
		RECT WindowRect;
		GetClientRect(hWindow, &WindowRect);

		//Try initialising the rendertarget
		EventResult = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWindow, D2D1::SizeU(WindowRect.right, WindowRect.bottom)),
			&RenderTarget);

		if (EventResult != S_OK) {
			return false;
		}

		//Globally create the solid brush for repeated use
		EventResult = RenderTarget->CreateSolidColorBrush(D2D1::ColorF(static_cast<float>(0.0), static_cast<float>(0.0), static_cast<float>(0.0), static_cast<float>(1.0)), &SolidBrush);

		if (EventResult != S_OK) {
			return false;
		}

		//Create the text factory
		EventResult = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&TextFactory));

		if (EventResult != S_OK) {
			return false;
		}

		return true;
	}
	void GraphicsEngine::BeginDraw() {
		RenderTarget->BeginDraw();
	}
	void GraphicsEngine::EndDraw() {
		RenderTarget->EndDraw();
	}
	void GraphicsEngine::ClearScreen(double R, double G, double B) {
		RenderTarget->Clear(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B)));
	}

	void GraphicsEngine::SetSolidBrush(double R, double G, double B, double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
	}

	void GraphicsEngine::SetLinearBrush(const std::vector<Graphics::Colour> GradientStops, const double StartX, const double StartY, const double EndX, const double EndY) {
		if (GradientStops.size() == 0) {
			throw std::invalid_argument("GraphicsEngine::SetLinearBrush: Empty GradientStops vector entered");
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
		switch (BrushToUse) {
		case UseSolidBrush:
			BrushSelection = SolidBrush;
			break;
		case UseLinearBrush:
			BrushSelection = LinearBrush;
			break;
		default:
			throw std::invalid_argument("GraphicsEngine: Invalid brush flag selected");
			break;
		}
		RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), BrushSelection, static_cast<float>(abs(Thickness)));
		//Remove access of brush selection to the brush it pointed to
		BrushSelection = nullptr;
		//if any repeatedly created brushes were used (ie linear and radial), delete them
		//Solid brush is created once and repeatedly used so do not release it here
		if (LinearBrush) {
			LinearBrush->Release();
			LinearBrush = nullptr;
		}
	}

	void GraphicsEngine::DrawFullCircle(double X, double Y, double Radius, double R, double G, double B, double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), SolidBrush);
	}

	void GraphicsEngine::DrawEmptyEllipse(double X, double Y, double RadiusA, double RadiusB, double R, double G, double B, double A, double Thickness) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(RadiusA), static_cast<float>(RadiusB)), SolidBrush, static_cast<float>(abs(Thickness)));
	}

	void GraphicsEngine::DrawFullEllipse(double X, double Y, double RadiusA, double RadiusB, double R, double G, double B, double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(RadiusA), static_cast<float>(RadiusB)), SolidBrush);
	}

	void GraphicsEngine::DrawEmptyRectangle(double X, double Y, double Width, double Height, double R, double G, double B, double A, double Thickness) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->DrawRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + Width), static_cast<float>(Y + Height)), SolidBrush, static_cast<float>(abs(Thickness)));
	}

	void GraphicsEngine::DrawFullRectangle(double X, double Y, double Width, double Height, double R, double G, double B, double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->FillRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + Width), static_cast<float>(Y + Height)), SolidBrush);
	}

	void GraphicsEngine::DrawEmptySquare(double X, double Y, double SideLength, double R, double G, double B, double A, double Thickness) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->DrawRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + SideLength), static_cast<float>(Y + SideLength)), SolidBrush, static_cast<float>(abs(Thickness)));
	}

	void GraphicsEngine::DrawFullSquare(double X, double Y, double SideLength, double R, double G, double B, double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		RenderTarget->FillRectangle(D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + SideLength), static_cast<float>(Y + SideLength)), SolidBrush);
	}

	void GraphicsEngine::WriteText(std::string TextIn, std::string FontName, double FontSize, double X, double Y, double Width, double Height, double R, double G, double B, double A) {
		SolidBrush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
		EventResult = TextFactory->CreateTextFormat((StringToWstring(FontName)).c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, static_cast<float>(abs(FontSize)), L"en-us", &TextFormat);
		if (EventResult != S_OK) {
			//Not thrown by invalid font name
			throw std::invalid_argument("GraphicsEngine::WriteText: Text format generation failure");
		}
		RenderTarget->DrawText((StringToWstring(TextIn)).c_str(), TextIn.size(), TextFormat, D2D1::Rect(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(X + Width), static_cast<float>(Y + Height)), SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	}
}