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

namespace Engine::ResourceSystem {
	class Scene;
}

namespace Engine::Rendering {
	class Renderer;
}

namespace Engine::Gui {

	class CHILLI_API GuiManager {
	public:
		static void Init();
		static void Shutdown();
		static void DrawEditorGui(Engine::Rendering::Renderer* renderer);
		static bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static ImGuiIO* GetIO();
	private:
		static int assetDropdownSelected;
		static const char* assetDropdownList[4];
		static const char* componentsList[5];
		static int assetFrameSelected;
		static int hierarchySelected;
		static int sceneSelected;
		static int entitySelected;
		static bool initialMousePos;
		static float mouseX;
		static float mouseY;
		static std::shared_ptr<Engine::ResourceSystem::Asset> selectedAsset;
		static std::shared_ptr<Engine::ECS::Entity> selectedEntity;
		static std::shared_ptr<Engine::ResourceSystem::Scene> selectedScene;
		static void BeginFrame();
		static void EndFrame();
		static void BuildMenuBar();
		static void BuildAssetManager();
		static void BuildSceneHierarchy();
		static void BuildEntityInspector();
		static void BuildScenePreviewWindow(Engine::Rendering::Renderer* renderer);
	};
}