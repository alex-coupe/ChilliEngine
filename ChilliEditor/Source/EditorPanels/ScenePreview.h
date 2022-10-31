#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"

namespace Chilli {
	class ScenePreview {
	public:
		ScenePreview();
		void BindFrameBuffer();
		void DrawGui();
		void Resize();
		float GetAspectRatio()const;
	private:
		std::unique_ptr<FrameBuffer> m_frameBuffer;
		float m_scenePreviewWindowWidth;
		float m_scenePreviewWindowHeight;
		bool m_initialMousePos = true;
		float m_mouseX;
		float m_mouseY;
	};
}