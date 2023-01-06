#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"

namespace Chilli {
	class CHILLI_API SpriteComponent : public Component {
	public:
		SpriteComponent(bool transparent = true, uint64_t texId = 0);
		SpriteComponent(SpriteComponent& rhs);
		bool& Transparent();
		uint64_t GetTexId()const;
		void SetTexId(uint64_t texId);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
	private:
		bool m_transparent;
		uint64_t m_texId;
	};
}