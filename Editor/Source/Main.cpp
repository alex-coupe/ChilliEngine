#include <iostream>
#include "Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Chilli::Core::Logger::Init();
	CHILLI_INFO("Hello World!");
	return 0;
}