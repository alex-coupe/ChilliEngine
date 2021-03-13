#pragma once
#include "../Core/ChilliDefinitions.h"

namespace Engine::ECS {

	class CHILLI_API Component {
	public:
		virtual ~Component() {};
		virtual void Serialize() = 0;
		virtual void Deserialize() = 0;
	private:
	};

}