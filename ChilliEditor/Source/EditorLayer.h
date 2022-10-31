#pragma once
#include "Core/Layer.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"
#include "EditorPanels/MenuBar.h"
#include "EditorPanels/ScenePreview.h"

namespace Chilli {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual void OnUpdate()override;
		virtual void OnRender()override;
		virtual void OnRenderGui()override;
		virtual void OnResize()override;
		virtual float GetDisplayAspectRatio()override;
	private:
		std::unique_ptr<Camera> m_editorCamera = nullptr;
		std::shared_ptr<MenuBar> m_menuBar;
		std::shared_ptr<ScenePreview> m_scenePreview;
	};
}