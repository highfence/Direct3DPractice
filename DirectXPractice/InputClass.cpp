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

	// 모든 키가 눌려있지 않은 상태로 초기화.
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// 키가 눌렸을 경우, true로 바꾸어준다.
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// 키가 떼졌을 경우, false로 바꾸어준다.
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// 인자에 해당하는 키가 눌렸는지 반환해준다.
	return m_keys[key];
}
