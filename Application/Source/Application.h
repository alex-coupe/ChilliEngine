#pragma once
#include <memory>
#include "Core\ChilliEngine.h"

class Application {
public:
	bool Init(HINSTANCE& hInstance);
	bool Update();
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};