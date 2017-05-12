#include "graphicsclass.h"
#include "D3DClass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// D3D 인스턴스 생성.
	m_pD3D = new D3DClass;
	if (!m_pD3D)
	{
		return false;
	}

	// D3D 인스턴스 초기화.
	result = m_pD3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Present the rendered scene to the screen.
	m_pD3D->EndScene();
	return true;
}


bool GraphicsClass::Render()
{

	return true;
}