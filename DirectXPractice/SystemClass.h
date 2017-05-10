#pragma once
// ���� �ð� ������ ���� ��ũ��.
// ������� �ʴ� API�� �Ϻθ� �����Ͽ� Windows ��� ������ ũ�⸦ ����.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// ���� ����.
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
