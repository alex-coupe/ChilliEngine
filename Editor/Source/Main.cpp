#include <Windows.h>
#include "Editor.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	
	//Create Editor On The Heap
	std::shared_ptr<Editor> ChilliEditor = std::make_shared<Editor>();

	//Initialize Engine Systems
	ChilliEditor->Init(hInstance);

	//Enter Main App Loop
	while (ChilliEditor->Update());

	return 0;
}