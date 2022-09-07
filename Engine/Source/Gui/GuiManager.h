#pragma once
#include "../Core/ChilliDefinitions.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <memory>
#include <vector>
#include <functional>
#include <Windows.h>
#include "../Nfd/include/nfd.h"
#include "../Core/DependencyResolver.h"
#include "../ResourceSystem/Asset.h"


namespace Engine::ECS {
	class Entity;
}

namespace Engine::Gui {

	class CHILLI_API GuiManager {
	public:
		static void Init();
		static void Shutdown();
		static void DrawEditorGui();
		static bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static ImGuiIO* GetIO();
	private:
		static int assetDropdownSelected;
		static const char* assetDropdownList[4];
		static int assetFrameSelected;
		static int hierarchySelected;
		static std::shared_ptr<Engine::ResourceSystem::Asset> selectedAsset;
		static std::shared_ptr<Engine::ECS::Entity> selectedEntity;
		static void BeginFrame();
		static void EndFrame();
		static void BuildMenuBar();
		static void BuildAssetManager();
		static void BuildSceneHierarchy();
		static void BuildEntityInspector();
		static void BuildScenePreviewWindow();
	};
}