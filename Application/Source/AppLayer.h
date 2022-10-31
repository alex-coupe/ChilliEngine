#pragma once
#include "Core/Layer.h"
#include "Core/DependencyResolver.h"
#include "Rendering/Renderer.h"

namespace Chilli {
	class AppLayer : public Layer {
	public:
		AppLayer();
		virtual void OnUpdate()override;
		virtual void OnRender()override;
		virtual void OnRenderGui()override;
		virtual void OnResize()override;
		virtual bool HasGui()override;
		virtual void OnSceneChange()override;
		virtual float GetDisplayAspectRatio()override;
	private:
		float m_screenWidth;
		float m_screenHeight;
	};
}