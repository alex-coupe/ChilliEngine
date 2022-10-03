#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../ECS/Component.h"

namespace Chilli {

	class CHILLI_API ScriptApi {
	public:
		static void Init();
	private:
		static void RegisterComponents();
		template <typename T>
		static void RegisterComponent(ComponentType type);
	};
}