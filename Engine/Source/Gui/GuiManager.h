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
		static void Init();
		static void Shutdown();
		static void DrawEditorGui();
		static bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static ImGuiIO* GetIO();
	private:
		static void BeginFrame();
		static void EndFrame();
	};
}