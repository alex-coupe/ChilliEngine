#include "Application.h"

int main()
{
	{
		std::unique_ptr<Chilli::Application> application = std::make_unique<Chilli::Application>();
		application->Run();
	}
	return 0;
}