#pragma once
#include "Core/Layer.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"
#include "EditorPanels/MenuBar.h"
#include "EditorPanels/ScenePreview.h"
#include "EditorPanels/AssetPanel.h"
#include "EditorPanels/SceneHierarchy.h"
#include "EditorPanels/EntityInspector.h"
#include "EditorPanels/ToolBar.h"
#include "EditorPanels/SplashPanel.h"

namespace Chilli {
	class EditorLayer : public Layer {
	public:
		EditorLayer(std::unique_ptr<Window>& window);
		virtual void OnUpdate()override;
		virtual void OnRender()override;
		virtual void OnRenderGui()override;
		virtual void OnSceneChange()override;
		virtual void OnResize()override;
		virtual bool HasGui()override;
		virtual float GetDisplayAspectRatio()override;
	private:
		std::unique_ptr<Camera> m_editorCamera = nullptr;
		std::shared_ptr<MenuBar> m_menuBar;
		std::shared_ptr<ScenePreview> m_scenePreview;
		std::shared_ptr<AssetPanel> m_assetPanel;
		std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
		std::shared_ptr<EntityInspector> m_entityInspector;
		std::shared_ptr<ToolBar> m_toolBar;
		std::shared_ptr<SplashPanel> m_splashPanel;
	};
}