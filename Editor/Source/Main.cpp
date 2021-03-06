#include "Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine::Core::Logger::Init();
	Editor::Window window(hInstance, L"Chilli Engine");

	return 0;
}