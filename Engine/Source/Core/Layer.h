#pragma once
#include "ChilliDefinitions.h"

namespace Chilli {

	class CHILLI_API Layer {
	public:
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
	};
}