#include "Application.h"

int main()
{
	{
		std::unique_ptr<Application> application = std::make_unique<Application>();
		application->Run();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}