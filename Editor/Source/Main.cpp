#include "Editor.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//Enable Logging
	Engine::Core::Logger::Init();

	//Create Window On The Stack
	Window wnd(hInstance);

	//Create Editor On The Heap
	std::unique_ptr<Editor> ChilliEditor = std::make_unique<Editor>();

	//Initialize Engine Systems
	ChilliEditor->Init();

	//Enter Main App Loop
	while (wnd.Update())
	{
		ChilliEditor->Update();
	}

	return 0;
}