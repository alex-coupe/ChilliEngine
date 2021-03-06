#pragma once
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include "ChilliDefinitions.h"

namespace Engine::Core {

	class Logger {

	public:
		CHILLI_FUNC static void Init();
	
		CHILLI_FUNC inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return m_engineLogger; }
		CHILLI_FUNC inline static std::shared_ptr<spdlog::logger>& GetEditorLogger() { return m_editorLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_engineLogger;
		static std::shared_ptr<spdlog::logger> m_editorLogger;

	};
}



#define ENGINE_ERROR(...) ::Engine::Core::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_WARN(...)  ::Engine::Core::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_INFO(...)  ::Engine::Core::Logger::GetEngineLogger()->info(__VA_ARGS__)

#define EDITOR_ERROR(...) ::Engine::Core::Logger::GetEditorLogger()->error(__VA_ARGS__)
#define EDITOR_WARN(...)  ::Engine::Core::Logger::GetEditorLogger()->warn(__VA_ARGS__)
#define EDITOR_INFO(...)  ::Engine::Core::Logger::GetEditorLogger()->info(__VA_ARGS__)