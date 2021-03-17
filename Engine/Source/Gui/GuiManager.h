#pragma once
#include "../Core/ChilliDefinitions.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <vector>
#include <functional>
#include <Windows.h>

namespace Engine::Gui {

	class CHILLI_API GuiManager {
	public:
		GuiManager();
		~GuiManager();
		void Draw()const;
		bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void InitWindowsHook(void* handle);
		void AddGuiElement(std::function<void()> callback);
		ImGuiIO* GetIO();
		void InitDxHook(ID3D11Device* device, ID3D11DeviceContext* context);
	private:
		std::vector<std::function<void()>> m_guiFunctions;
	};
}