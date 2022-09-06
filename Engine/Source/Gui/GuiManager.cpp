#include "GuiManager.h"
#include "../ResourceSystem/ProjectManager.h"

void Engine::Gui::GuiManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

void Engine::Gui::GuiManager::Shutdown()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void Engine::Gui::GuiManager::DrawEditorGui()
{
	BeginFrame();
	BuildMenuBar();
	EndFrame();
}

void Engine::Gui::GuiManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Engine::Gui::GuiManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Gui::GuiManager::BuildMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) 
			{
				Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->NewProject();
			}

			if (ImGui::MenuItem("Open", "Ctrl+O")) 
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);
				if (result == NFD_OKAY) 
				{	
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->LoadProject(outPath);
					free(outPath);
				}
			}

			if (ImGui::MenuItem("Save", "Ctrl+S")) 
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_SaveDialog("json", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SaveProject(outPath);
					free(outPath);
				}

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y")) {} 
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Engine::Gui::GuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

ImGuiIO* Engine::Gui::GuiManager::GetIO()
{
	if (ImGui::GetCurrentContext())
	{
		return &ImGui::GetIO();
	}
	return nullptr;
}



