#include "Editor.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	std::unique_ptr<Editor> ChilliEditor = std::make_unique<Editor>(hInstance);
	ChilliEditor->Init();
	return 0;
}