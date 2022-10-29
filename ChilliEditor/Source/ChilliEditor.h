#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace Chilli {

	class CHILLI_API ChilliEditor {
	public:
		ChilliEditor();
		~ChilliEditor();
		ChilliEditor(const ChilliEditor&) = delete;
		ChilliEditor& operator=(const ChilliEditor&) = delete;
		ChilliEditor(ChilliEditor&&) = default;
		void Run();
	private:
		std::unique_ptr<ChilliEngine> m_engine = nullptr;
	};
}