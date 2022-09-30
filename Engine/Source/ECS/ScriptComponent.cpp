#include "ScriptComponent.h"
#include "../ResourceSystem/ProjectManager.h"
namespace Chilli {
	ScriptComponent::ScriptComponent()
		: Component(ComponentTypes::Script, "Script")
	{}

	ScriptComponent::ScriptComponent(const std::string & scriptName)
		: Component(ComponentTypes::Script, "Script"),m_scriptName(scriptName)
	{}

	ScriptComponent::ScriptComponent(const ScriptComponent& rhs)
		: Component(ComponentTypes::Script, "Script")
	{
		m_scriptName = rhs.m_scriptName;
	}

	const std::string ScriptComponent::Serialize() const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type)
			<< ",\"ScriptClassName\":\"" << m_scriptName
			<< "\"}";
		return  ss.str();
	}

	const std::string& ScriptComponent::GetScriptName()const
	{
		return m_scriptName;
	}

	void ScriptComponent::SetScript(const std::string& script)
	{
		m_scriptName = script;
	}
}