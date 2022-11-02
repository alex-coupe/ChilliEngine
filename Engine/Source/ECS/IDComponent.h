#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"

namespace Chilli {

	class CHILLI_API IDComponent : public Component {
	public:
		IDComponent(const std::string& entityName, const std::string& entityTag = "");
		IDComponent(IDComponent& rhs);
		const std::string& GetName()const;
		void SetName(const std::string& name);
		const std::string& GetTag()const;
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		void SetTag(const std::string& tag);
	private:
		std::string m_entityName;
		std::string m_entityTag;
	};
}
