#pragma once
#include "ChilliDefinitions.h"

namespace Chilli {
	enum class LayerType {
		Editor, App
	};
	class CHILLI_API Layer {
	public:
		Layer(LayerType type) :
			m_layerType(type)
		{}
		const LayerType GetLayerType()const
		{
			return m_layerType;
		}
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnRenderGui() = 0;
		virtual void OnOpen() = 0;
		virtual void OnResize() = 0;
		virtual bool HasGui() = 0;
		virtual void OnSceneChange() = 0;
		virtual float GetDisplayAspectRatio() = 0;
	protected:
		LayerType m_layerType;
	};
}