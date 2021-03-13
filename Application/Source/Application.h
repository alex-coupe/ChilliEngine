#pragma once
#include <memory>
#include "Core\ChilliEngine.h"

class Application {
public:
	Application(HINSTANCE& hInstance);
	~Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = default;
	void Update();
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};