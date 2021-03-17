#include "Application.h"
#include <Windows.h>


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	{
		std::unique_ptr<Application> application = std::make_unique<Application>(hInstance);

		//Enter Main App Loop
		application->Update();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}