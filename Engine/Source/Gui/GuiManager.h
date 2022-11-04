#pragma once
#include "../Core/ChilliDefinitions.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGuizmo/ImGuizmo.h"
#include <memory>
#include <vector>
#include <functional>
#include <Windows.h>
#include "../Nfd/include/nfd.h"
#include "../Core/DependencyResolver.h"
#include "../ResourceSystem/Asset.h"

namespace Chilli {

	class CHILLI_API GuiManager {
	public:
		static void Init();
		static void Shutdown();
		static bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static ImGuiIO* GetIO();
		static ImGuiContext* GetContext();
		static void BeginFrame();
		static void EndFrame();
	private:		
	};
}