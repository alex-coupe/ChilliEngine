#pragma once
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include "ChilliDefinitions.h"

namespace Engine::Core {

	class Logger {

	public:
		CHILLI_CLASS static void Init();
	
		CHILLI_CLASS inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_logger; }

	private:
		static std::shared_ptr<spdlog::logger> m_logger;

	};
}



#define CHILLI_ERROR(...) ::Engine::Core::Logger::GetLogger()->error(__VA_ARGS__)
#define CHILLI_WARN(...)  ::Engine::Core::Logger::GetLogger()->warn(__VA_ARGS__)
#define CHILLI_INFO(...)  ::Engine::Core::Logger::GetLogger()->info(__VA_ARGS__)