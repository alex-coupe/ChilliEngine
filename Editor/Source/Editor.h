#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
class Editor {
public:
	void Init(HWND& handle);
	void Update();
	void RaiseEvent(EventData& data);
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};