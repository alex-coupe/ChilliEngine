#pragma once
#include "Window.h"

class Editor {
public:
	void Init();
	void Update();
private:
	std::unique_ptr<ChilliEngine> m_Engine = nullptr;
};