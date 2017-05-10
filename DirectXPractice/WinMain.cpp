#include "SystemClass.h"

/*
	윈도우 API 시작점.
	시스템 클래스를 생성하고 Run 메소드를 호출해 준다.
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpszCmdParam, int nCmdShow)
{
	SystemClass* System;
	bool result;

	// 시스템 오브젝트 생성.
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// 시스템 오브젝트 초기화.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// App이 끝난 경우, System을 꺼준다.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
