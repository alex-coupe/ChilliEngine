#include "IDComponent.h"

namespace Chilli {
	IDComponent::IDComponent(const std::string& entityName, const std::string& entityTag)
		:Component(ComponentType::ID, "ID"), m_entityName(entityName), m_entityTag(entityTag)
	{
	}

	IDComponent::IDComponent(IDComponent& rhs)
		:Component(rhs.m_type,rhs.m_name)
	{
		m_entityName = rhs.m_entityName;
		m_entityTag = rhs.m_entityTag;
	}

	void IDComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<IDComponent>(rhs);
		m_entityName = comp->m_entityName;
		m_entityTag = comp->m_entityTag;
	}

	const std::string& IDComponent::GetName() const
	{
		return m_entityName;
	}

	void IDComponent::SetName(const std::string& name)
	{
		m_entityName = name;
	}

	const std::string& IDComponent::GetTag()const
	{
		return m_entityTag;
	}

	void IDComponent::SetTag(const std::string& tag)
	{
		m_entityTag = tag;
	}

	const std::string IDComponent::Serialize(uint64_t entityId)const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"EntName\": \""  << m_entityName << "\", \"EntTag\": \"" << m_entityTag << "\"}";
		return  ss.str();
	}
}
