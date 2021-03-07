#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
class Editor {
public:
	void Init();
	void Update();
	void RaiseMouseEvent();
	void RaiseKeyboardEvent();
	void RaiseSystemEvent();
private:
	std::unique_ptr<ChilliEngine> m_engine = nullptr;
};