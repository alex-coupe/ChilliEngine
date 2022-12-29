#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"

namespace Chilli {
	class CHILLI_API SpriteComponent : public Component {
	public:
		SpriteComponent(bool transparent = true);
		SpriteComponent(SpriteComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
	private:
		bool m_transparent;
	};
}