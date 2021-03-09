#include "Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//Enable Logging
	Engine::Core::Logger::Init();

	//Create Editor On The Heap
	std::shared_ptr<Editor> ChilliEditor = std::make_shared<Editor>();

	//Create Window On The Stack And Give Reference To Editor
	Window wnd(hInstance, ChilliEditor);

	//Initialize Engine Systems
	ChilliEditor->Init(wnd.GetHandle());

	wnd.DisplayWindow();

	//Enter Main App Loop
	while (wnd.Update())
	{
		ChilliEditor->Update();
	}

	return 0;
}