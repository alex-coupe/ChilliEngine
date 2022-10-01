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

	const std::string ScriptComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type)
			<< ",\"ScriptClassName\":\"" << m_scriptName << "\"";

		const auto& scriptInstance = ScriptInstanceRepository::GetScriptInstanceByEntityId(entityId);
		if (scriptInstance && scriptInstance->HasFields())
		{
			ss << ",\"Fields\":[";
			const auto& scriptFields = scriptInstance->GetFields();
			for (auto i = scriptFields.begin(); i != scriptFields.end(); i++)
			{
				ss << "{\"Name\":\"" << i->second.Name << "\",\"Type\":" << (int)i->second.Type
					<< ",\"Value\":";
				switch (i->second.Type)
				{
				case FieldType::Float:
					ss << scriptInstance->GetFieldValue<float>(i->first);
					break;
				case FieldType::Bool:
					ss << scriptInstance->GetFieldValue<bool>(i->first);
					break;
				case FieldType::Byte:
					ss << scriptInstance->GetFieldValue<uint8_t>(i->first);
					break;
				case FieldType::Char:
					ss << scriptInstance->GetFieldValue<int8_t>(i->first);
					break;
				case FieldType::Double:
					ss << scriptInstance->GetFieldValue<double>(i->first);
					break;
				case FieldType::Entity:
				case FieldType::ULong:
					ss << scriptInstance->GetFieldValue<uint64_t>(i->first);
					break;
				case FieldType::Int:
					ss << scriptInstance->GetFieldValue<int32_t>(i->first);
					break;
				case FieldType::Long:
					ss << scriptInstance->GetFieldValue<int64_t>(i->first);
					break;
				case FieldType::Short:
					ss << scriptInstance->GetFieldValue<int16_t>(i->first);
					break;
				case FieldType::UInt:
					ss << scriptInstance->GetFieldValue<uint32_t>(i->first);
					break;
				case FieldType::UShort:
					ss << scriptInstance->GetFieldValue<uint16_t>(i->first);
					break;
				default:
					ss << "Unknown";
					break;
				}
				ss << "},";
			}
			ss.seekp(-1, std::ios_base::end);
		}
		ss	<< "]}";
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