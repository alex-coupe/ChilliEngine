#include "ScriptComponent.h"
#include "../ResourceSystem/ProjectManager.h"
namespace Chilli {
	ScriptComponent::ScriptComponent()
		: Component(ComponentType::Script, "Script")
	{}

	ScriptComponent::ScriptComponent(const std::string & scriptName)
		: Component(ComponentType::Script, "Script"),m_scriptName(scriptName)
	{}

	ScriptComponent::ScriptComponent(const ScriptComponent& rhs)
		: Component(ComponentType::Script, "Script")
	{
		m_scriptName = rhs.m_scriptName;
	}

	void ScriptComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto comp = std::static_pointer_cast<ScriptComponent>(rhs);
		m_scriptName = comp->m_scriptName;
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
				if (scriptInstance->HasField(i->first))
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
					case FieldType::Vector2:
					{
						auto values = scriptInstance->GetFieldValue<DirectX::XMFLOAT2>(i->first);
						ss << "[{\"X\":" << values.x << ", \"Y\":" << values.y << "}]";
					}
					break;
					case FieldType::Vector3:
					{
						auto values = scriptInstance->GetFieldValue<DirectX::XMFLOAT3>(i->first);
						ss << "[{\"X\":" << values.x << ", \"Y\":" << values.y << ",\"Z\":" << values.z << "}]";
					}
					break;
					case FieldType::Vector4:
					{
						auto values = scriptInstance->GetFieldValue<DirectX::XMFLOAT4>(i->first);
						ss << "[{\"X\":" << values.x << ", \"Y\":" << values.y << ",\"Z\":" << values.z << ", \"W\":" << values.w << "}]";
					}
					break;
					default:
						ss << "Unknown";
						break;
					}
					ss << "},";
				}
				else
					ss << ",";
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