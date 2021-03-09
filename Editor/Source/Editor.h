#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
class Editor {
public:
	void Init(WindowData& window_data);
	void Update();
	void RaiseEvent(EventData& data);
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};