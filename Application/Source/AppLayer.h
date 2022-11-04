#pragma once
#include "Core/Layer.h"
#include "Core/DependencyResolver.h"
#include "Rendering/Renderer.h"
#include "Core/Window.h"

namespace Chilli {
	class AppLayer : public Layer {
	public:
		AppLayer(std::unique_ptr<Window>& window);
		virtual void OnUpdate()override;
		virtual void OnRender()override;
		virtual void OnRenderGui()override;
		virtual void OnResize()override;
		virtual void OnOpen()override;
		virtual bool HasGui()override;
		virtual void OnSceneChange()override;
		virtual float GetDisplayAspectRatio()override;
	private:
		const std::unique_ptr<Window>& m_window;
		float m_screenWidth;
		float m_screenHeight;
	};
}