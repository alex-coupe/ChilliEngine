#pragma once
#include "Window.h"

class Editor {
public:
	Editor(HINSTANCE& instance);
	void Init();
	void Update();
private:
	std::unique_ptr<ChilliEngine> m_Engine = nullptr;
	std::unique_ptr<Window> m_Window = nullptr;
};