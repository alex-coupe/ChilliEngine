#include "Logger.h"
#include <fcntl.h>

namespace Chilli::Core {

	std::shared_ptr<spdlog::logger> Logger::m_logger;

	void Logger::Init()
	{
		AllocConsole();
		HANDLE stdHandle;
		int hConsole;
		FILE* fp;
		stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		hConsole = _open_osfhandle((long long)stdHandle, _O_TEXT);
		fp = _fdopen(hConsole, "w");

		freopen_s(&fp, "CONOUT$", "w", stdout);
		m_logger = spdlog::stdout_color_mt("chilli engine");
		m_logger->set_level(spdlog::level::trace);
		spdlog::set_pattern("%^[%T] %n %l: %v%$");
	}

}