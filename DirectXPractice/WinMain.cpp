#include "SystemClass.h"

/*
	������ API ������.
	�ý��� Ŭ������ �����ϰ� Run �޼ҵ带 ȣ���� �ش�.
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpszCmdParam, int nCmdShow)
{
	SystemClass* System;
	bool result;

	// �ý��� ������Ʈ ����.
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// �ý��� ������Ʈ �ʱ�ȭ.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// App�� ���� ���, System�� ���ش�.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
