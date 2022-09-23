#include "ScriptComponent.h"
#include "../ResourceSystem/ProjectManager.h"

Engine::ECS::ScriptComponent::ScriptComponent(const Engine::Utilities::UUID& uuid)
	: Component(Engine::ECS::ComponentTypes::Script, "Script"), m_scriptUuid(uuid)
{
	
}

Engine::ECS::ScriptComponent::ScriptComponent(const ScriptComponent& rhs)
	: Component(Engine::ECS::ComponentTypes::Script, "Script")
{
	m_scriptName = rhs.m_scriptName;
	m_scriptUuid = rhs.m_scriptUuid;
}

const std::string Engine::ECS::ScriptComponent::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) 
		<< ", \"ScriptUuid\":\"" << m_scriptUuid.GetUUID() 
		<< ",\"ScriptClassName\":\"" << m_scriptName
		<< "\"}";
	return  ss.str();
}

const std::string& Engine::ECS::ScriptComponent::GetScriptName()const
{
	return m_scriptName;
}

void Engine::ECS::ScriptComponent::SetScript(const std::string& script)
{
	m_scriptName = script;
}