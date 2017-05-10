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

	// ��ũ�� ũ�� ���� �ʱ�ȭ.
	screenWidth = 0;
	screenHeight = 0;

	// ������ API �ʱ�ȭ.
	InitializeWindows(screenWidth, screenHeight);

	// Ű���� �Է��� ����ϴ� InputClass ����.
	m_pInput = new InputClass;
	if (!m_pInput)
	{
		return false;
	}
	
	// InputClass �ʱ�ȭ.
	m_pInput->Initialize();

	// �׷��� �ڵ鸵�� ����� GraphicsClass ����.
	m_pGraphics = new GraphicsClass;
	if (!m_pGraphics)
	{
		return false;
	}

	// GraphcisClass �ʱ�ȭ.
	result = m_pGraphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// �׷��� Ŭ���� ����.
	if (m_pGraphics)
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = 0;
	}

	// ��ǲ Ŭ���� ����.
	if (m_pInput)
	{
		delete m_pInput;
		m_pInput = 0;
	}

	ShutdownWindows();

	return;
}

/*
	WinAPI �޽��� ������ ����ϴ� �Լ�.
*/
void SystemClass::Run()
{
	// �޽��� ����ü �ʱ�ȭ.
	MSG msg;
	bool result;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		// �޽��� �ڵ鸵.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����쿡�� �� �޽����� WM_QUIT��� ���� Ż��.
		if (msg.message == WM_QUIT)
		{
			break;
		}
		// �׷��� �ʴٸ�, Frame �Լ� ȣ��.
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
	���ø����̼ǿ� ���� ó���� ����ϴ� �Լ�.
	InputClass�� Ȯ���Ͽ� VK_ESCAPE�� ���ȴٸ� Run()�� �����ϵ��� false ��ȯ.
	���ᰡ �ȵǾ��ٸ� GrphicsClass�� Frame�� ȣ�����ش�.
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
	���� �����ִ� �޽����� ���ؼ� �ڵ鸵���ִ� �Լ�.
	WndProc���� �Ѿ�´�.
*/
LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{

	// Ű���� �ٿ� �޽���.
	case WM_KEYDOWN:
	{
		// ��ǲ ������Ʈ�� ������ �ǳ��ش�.
		m_pInput->KeyDown((unsigned int)wParam);
		return 0;
	}

	// Ű���� �� �޽���.
	case WM_KEYUP:
	{
		// ��ǲ ������Ʈ�� ������ �ǳ��ش�.
		m_pInput->KeyUp((unsigned int)wParam);
		return 0;
	}

	// �� ���� �޽����� �⺻ ó�� �����ι��� �־��ش�.
	default:
	{
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	}
}

/*
	������ �ʱ�ȭ �Լ�.
	���ڷ� �� int �ڷ����� ��ũ�� ���̸� ��ȯ�Ͽ� ���ø����̼ǿ��� ����� �� �ֵ��� �Ѵ�.
*/
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX WndClass;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺο��� �� Ŭ������ �ڵ鸵 �� �� �ֵ��� �����͸� ������ش�.
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� �޾ƿ´�.
	m_hInstance = GetModuleHandle(NULL);

	// ���ø����̼� �̸� ���.
	m_ApplicationName = L"Engine";

	// ������ Ŭ���� ����ü ���� ���.
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

	// ������ ����ü ���.
	RegisterClassEx(&WndClass);

	// Ŭ���̾�Ʈ �ػ� ��ȯ.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// �����찡 FULL_SCREEN ������ ���.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// FULL_SCREEN �������� ������ ������ �ٲ��ش�.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� �»�� ��ǥ�� ������ش�.
		posX = posY = 0;
	}
	else
	{
		// �ػ󵵰� 800 * 600.
		screenWidth = 800;
		screenHeight = 600;

		// �����츦 ȭ���� �߾����� ��ġ ��Ų��.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ����.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	// ShowWindow�� ȣ�����ְ� ��Ŀ���� �ش�.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// ���콺 Ŀ���� ����� ���.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� �ٽ� �����ش�.
	ShowCursor(true);

	// Ǯ ��ũ�� ��忡�� �����Ƿ� ȭ�� ������ �ٽ� ���ش�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// �����츦 �ݴ´�.
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	// ���ø����̼� �ν��Ͻ��� �����Ѵ�.
	UnregisterClass(m_ApplicationName, m_hInstance);
	m_hInstance = nullptr;

	// �ܺ� ���� �����͸� �������ش�.
	ApplicationHandle = nullptr;

	return;
}

/*
	�⺻���� �޽����� �ڵ鸵���ִ� �Լ�.
	�⺻���� �޽����� ���� Ȯ���� ������ SystemClass�� MessageHandler�� �������ش�.
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

	// SystemClass�� �޽��� �ڵ鷯�� ����.
	default:
	{
		return ApplicationHandle->MessageHandler(hWnd, iMessage, wParam, lParam);
	}
	}
}
