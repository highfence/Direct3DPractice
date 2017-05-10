#pragma once

/*
	현재 코드의 용이함을 위해 DirectInput을 배우기 전까지 기본 윈도우 입력을 사용.
*/

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};