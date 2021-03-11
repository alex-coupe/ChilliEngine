#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
#include "Window.h"

class Application {
public:
	bool Init(HINSTANCE& hInstance);
	void Update();
private:
	std::shared_ptr<ChilliEngine> m_engine = nullptr;
	std::unique_ptr<Window> m_window = nullptr;
};