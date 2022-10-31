#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
#include "AppLayer.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
namespace Chilli {
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
}