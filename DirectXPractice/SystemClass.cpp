#include "SystemClass.h"
#include "InputClass.h"
#include "GraphicsClass.h"

SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// 스크린 크기 변수 초기화.
	screenWidth = 0;
	screenHeight = 0;

	// 윈도우 API 초기화.
	InitializeWindows(screenWidth, screenHeight);

	// 키보드 입력을 담당하는 InputClass 생성.
	m_pInput = new InputClass;
	if (!m_pInput)
	{
		return false;
	}
	
	// InputClass 초기화.
	m_pInput->Initialize();

	// 그래픽 핸들링을 담당할 GraphicsClass 생성.
	m_pGraphics = new GraphicsClass;
	if (!m_pGraphics)
	{
		return false;
	}

	// GraphcisClass 초기화.
	result = m_pGraphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// 그래픽 클래스 해제.
	if (m_pGraphics)
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = 0;
	}

	// 인풋 클래스 해제.
	if (m_pInput)
	{
		delete m_pInput;
		m_pInput = 0;
	}

	ShutdownWindows();

	return;
}

/*
	WinAPI 메시지 루프를 담당하는 함수.
*/
void SystemClass::Run()
{
	// 메시지 구조체 초기화.
	MSG msg;
	bool result;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		// 메시지 핸들링.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우에서 온 메시지가 WM_QUIT라면 루프 탈출.
		if (msg.message == WM_QUIT)
		{
			break;
		}
		// 그렇지 않다면, Frame 함수 호출.
		else
		{
			result = Frame();
			if (!result)
			{
				break;
			}
		}
	}
	return;
}

/*
	어플리케이션에 대한 처리를 담당하는 함수.
	InputClass를 확인하여 VK_ESCAPE가 눌렸다면 Run()을 종료하도록 false 반환.
	종료가 안되었다면 GrphicsClass의 Frame을 호출해준다.
*/
bool SystemClass::Frame()
{
	bool result;

	if (m_pInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = m_pGraphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

/*
	내가 관심있는 메시지에 대해서 핸들링해주는 함수.
	WndProc에서 넘어온다.
*/
LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{

	// 키보드 다운 메시지.
	case WM_KEYDOWN:
	{
		// 인풋 오브젝트에 정보를 건네준다.
		m_pInput->KeyDown((unsigned int)wParam);
		return 0;
	}

	// 키보드 업 메시지.
	case WM_KEYUP:
	{
		// 인풋 오브젝트에 정보를 건네준다.
		m_pInput->KeyUp((unsigned int)wParam);
		return 0;
	}

	// 그 외의 메시지는 기본 처리 디파인문에 넣어준다.
	default:
	{
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	}
}

/*
	윈도우 초기화 함수.
	인자로 들어간 int 자료형에 스크린 길이를 반환하여 어플리케이션에서 사용할 수 있도록 한다.
*/
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX WndClass;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부에서 이 클래스를 핸들링 할 수 있도록 포인터를 등록해준다.
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 받아온다.
	m_hInstance = GetModuleHandle(NULL);

	// 어플리케이션 이름 등록.
	m_ApplicationName = L"Engine";

	// 윈도우 클래스 구조체 정보 등록.
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = m_hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	WndClass.hIconSm = WndClass.hIcon;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = m_ApplicationName;
	WndClass.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 구조체 등록.
	RegisterClassEx(&WndClass);

	// 클라이언트 해상도 반환.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 윈도우가 FULL_SCREEN 세팅일 경우.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// FULL_SCREEN 세팅으로 윈도우 설정을 바꿔준다.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 좌상단 좌표를 등록해준다.
		posX = posY = 0;
	}
	else
	{
		// 해상도가 800 * 600.
		screenWidth = 800;
		screenHeight = 600;

		// 윈도우를 화면의 중앙으로 위치 시킨다.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 윈도우 생성.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	// ShowWindow를 호출해주고 포커스를 준다.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// 마우스 커서를 숨기는 기능.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// 마우스 커서를 다시 보여준다.
	ShowCursor(true);

	// 풀 스크린 모드에서 나가므로 화면 세팅을 다시 해준다.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 윈도우를 닫는다.
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	// 어플리케이션 인스턴스를 해제한다.
	UnregisterClass(m_ApplicationName, m_hInstance);
	m_hInstance = nullptr;

	// 외부 접근 포인터를 해제해준다.
	ApplicationHandle = nullptr;

	return;
}

/*
	기본적인 메시지를 핸들링해주는 함수.
	기본적인 메시지에 대한 확인이 끝나면 SystemClass의 MessageHandler로 연결해준다.
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// SystemClass의 메시지 핸들러로 연결.
	default:
	{
		return ApplicationHandle->MessageHandler(hWnd, iMessage, wParam, lParam);
	}
	}
}
