#include "InputClass.h"


InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize()
{
	int i;

	// ��� Ű�� �������� ���� ���·� �ʱ�ȭ.
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ������ ���, true�� �ٲپ��ش�.
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// Ű�� ������ ���, false�� �ٲپ��ش�.
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// ���ڿ� �ش��ϴ� Ű�� ���ȴ��� ��ȯ���ش�.
	return m_keys[key];
}
