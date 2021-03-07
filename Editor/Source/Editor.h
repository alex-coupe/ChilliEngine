#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
class Editor {
public:
	void Init();
	void Update();
	void RaiseMouseEvent(EventData& data);
	void RaiseKeyboardEvent(EventData& data);
	void RaiseSystemEvent(EventData& data);
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};