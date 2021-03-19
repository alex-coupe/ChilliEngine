#include "GuiManager.h"


Engine::Gui::GuiManager::GuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

Engine::Gui::GuiManager::~GuiManager()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void Engine::Gui::GuiManager::BeginFrame() const
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Engine::Gui::GuiManager::EndFrame() const
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Gui::GuiManager::Draw() const
{
	
		for (const auto& callback: m_guiFunctions)
		{
			callback();
		}

}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Engine::Gui::GuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void Engine::Gui::GuiManager::InitWindowsHook(void* handle)
{
	ImGui_ImplWin32_Init(handle);
}

void Engine::Gui::GuiManager::AddGuiElement(std::function<void()> callback)
{
	m_guiFunctions.push_back(callback);
}

ImGuiIO* Engine::Gui::GuiManager::GetIO()
{
	if (ImGui::GetCurrentContext())
	{
		return &ImGui::GetIO();
	}
	return nullptr;
}

void Engine::Gui::GuiManager::InitDxHook(ID3D11Device* device, ID3D11DeviceContext* context)
{
	ImGui_ImplDX11_Init(device, context);
}


