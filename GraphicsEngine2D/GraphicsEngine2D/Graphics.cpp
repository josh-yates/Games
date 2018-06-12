#include "Graphics.h"

GraphicsEngine::GraphicsEngine() {
	Factory = NULL;
	RenderTarget = NULL;
}

GraphicsEngine::~GraphicsEngine() {
	//Release resources
	if (Factory) {
		Factory->Release();
	}
	if (RenderTarget) {
		RenderTarget->Release();
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
	ID2D1SolidColorBrush* CircleBrush;
	RenderTarget->CreateSolidColorBrush(D2D1::ColorF(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A)), &CircleBrush);
	RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), CircleBrush, 3);
	RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(static_cast<float>(X), static_cast<float>(Y)), static_cast<float>(Radius), static_cast<float>(Radius)), CircleBrush);
	CircleBrush->Release();
}