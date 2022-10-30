#pragma once
#include "Core/Layer.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"
#include "EditorPanels/MenuBar.h"
#include "Gui/ImGui/imgui.h"
#include "Gui/ImGui/imgui_impl_dx11.h"
#include "Gui/ImGui/imgui_impl_win32.h"

namespace Chilli {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual void OnUpdate()override;
		virtual void OnRender()override;
	private:
		std::unique_ptr<Camera> m_editorCamera = nullptr;
		std::vector<std::shared_ptr<EditorPanel>> m_editorPanels;
	};
}