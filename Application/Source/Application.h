#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
class Application {
public:
	Application();
	~Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = default;
	void Run();
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};