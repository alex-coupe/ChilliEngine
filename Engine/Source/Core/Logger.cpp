#include "Logger.h"
#include <fcntl.h>

namespace Engine::Core {

	std::shared_ptr<spdlog::logger> Logger::m_engineLogger;
	std::shared_ptr<spdlog::logger> Logger::m_editorLogger;

	void Logger::Init()
	{
		
		AllocConsole();
		m_engineLogger = spdlog::stdout_color_mt("Chilli Engine");
		m_engineLogger->set_level(spdlog::level::trace);
		m_editorLogger = spdlog::stdout_color_mt("Chilli Editor");
		m_editorLogger->set_level(spdlog::level::trace);
		spdlog::set_pattern("%^[%T] %n %l: %v%$");

		ENGINE_INFO("Engine Logger Initialized");
		EDITOR_INFO("Editor Logger Initialized");
	}

	CHILLI_API void Logger::Kill()
	{
		FreeConsole();
	}

}