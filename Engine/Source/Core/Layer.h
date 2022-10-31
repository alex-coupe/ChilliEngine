#pragma once
#include "ChilliDefinitions.h"

namespace Chilli {

	class CHILLI_API Layer {
	public:
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnRenderGui() = 0;
		virtual void OnResize() = 0;
		virtual float GetDisplayAspectRatio() = 0;
	};
}