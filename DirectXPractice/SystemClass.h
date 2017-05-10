#pragma once
// 빌드 시간 단축을 위한 매크로.
// 사용하지 않는 API의 일부를 제외하여 Windows 헤더 파일의 크기를 줄임.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 전방 선언.
class InputClass;
class GraphicsClass;

/*
*/

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_hInstance = nullptr;
	HWND m_hWnd = nullptr;

	InputClass* m_pInput = nullptr;
	GraphicsClass* m_pGraphics = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = nullptr;
