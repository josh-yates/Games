#include "Graphics.h"

GraphicsEngine::GraphicsEngine() {
	Factory = NULL;
	RenderTarget = NULL;
	Brush = NULL;
}

GraphicsEngine::~GraphicsEngine() {
	//Release resources
	if (Factory) {
		Factory->Release();
	}
	if (RenderTarget) {
		RenderTarget->Release();
	}
	if (Brush) {
		Brush->Release();
	}
}

bool GraphicsEngine::Init(HWND hWindow) {
	//Try initialising the factory
	HRESULT InitResult{ D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory) };
	if (InitResult != S_OK) {
		return false;
	}

	//Read the size of the window
	RECT WindowRect;
	GetClientRect(hWindow, &WindowRect);

	//Try initialising the rendertarget
	InitResult = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), 
		D2D1::HwndRenderTargetProperties(hWindow, D2D1::SizeU(WindowRect.right, WindowRect.bottom)), 
		&RenderTarget);

	if (InitResult != S_OK) {
		return false;
	}

	//Globally create the brush for repeated use
	InitResult = RenderTarget->CreateSolidColorBrush(D2D1::ColorF(static_cast<float>(0.0), static_cast<float>(0.5), static_cast<float>(0.7), static_cast<float>(1.0)), &Brush);
	
	if (InitResult != S_OK) {
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
void GraphicsEngine::DrawCircle(double X, double Y, double Radius, double R, double G, double B, double A) {
	Brush->SetColor(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)));
	//RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), Brush, 3);
	RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), Brush);
}