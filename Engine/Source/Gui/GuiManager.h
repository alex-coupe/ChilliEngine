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

namespace Chilli {

	class Entity;
	class Scene;
	class Renderer;

	class CHILLI_API GuiManager {
	public:
		static void Init();
		static void Shutdown();
		static void DrawEditorGui(Renderer* renderer);
		static bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static ImGuiIO* GetIO();
		static ImGuiContext* GetContext();
		static float GetPreviewWindowAspectRatio();
		static void BeginFrame();
		static void EndFrame();
	private:
		static int assetDropdownSelected;
		static const char* assetTypesList[6];
		static const char* componentsList[18];
		static uint64_t assetFrameSelected;
		static uint64_t hierarchySelected;
		static uint64_t sceneSelected;
		static uint64_t entitySelected;
		static bool initialMousePos;
		static float mouseX;
		static float mouseY;
		static float scenePreviewWindowWidth;
		static float scenePreviewWindowHeight;
		static std::shared_ptr<Asset> selectedAsset;
		static std::shared_ptr<Entity> selectedEntity;
		static std::shared_ptr<Scene> selectedScene;
		
		static void BuildMenuBar();
		static void BuildAssetManager();
		static void BuildSceneHierarchy();
		static void BuildEntityInspector();
		static void BuildScenePreviewWindow(Renderer* renderer);
	};
}