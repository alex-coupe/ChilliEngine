#include "Editor.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine::Core::Logger::Init();
	Window wnd(hInstance);
	std::unique_ptr<Editor> ChilliEditor = std::make_unique<Editor>();
	ChilliEditor->Init();
	while (wnd.Update())
	{
		ChilliEditor->Update();
	}
	return 0;
}