#pragma once
#include <memory>
#include "Core\ChilliEngine.h"

class Application {
public:
	Application(HINSTANCE& hInstance);
	~Application();
	void Update();
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};