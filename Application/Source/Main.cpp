#include <Windows.h>
#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//Enable Logging
	Engine::Core::Logger::Init();

	//Create Editor On The Heap
	std::shared_ptr<Application> application = std::make_shared<Application>();

	//Initialize Engine Systems
	application->Init(hInstance);

	//Enter Main App Loop
	application->Update();

	ImGui::DestroyContext();

	return 0;
}