#include "ScriptInstance.h"
#include "ScriptEngine.h"
#define PUBLIC_FIELD_FLAG 6
#define PRIVATE_FIELD_FLAG 1

namespace Chilli {

	ScriptInstance::ScriptInstance(MonoClass* scriptClass, uint64_t entityId)
	{
		m_monoObject = mono_object_new(ScriptEngine::GetAppDomain(), scriptClass);
		mono_runtime_object_init(m_monoObject);
        auto coreClass = mono_class_from_name(ScriptEngine::GetCoreAssemblyImage(), "Chilli", "ChilliScript");
        auto constructor = mono_class_get_method_from_name(coreClass, ".ctor", 1);
        void* param = &entityId;
        mono_runtime_invoke(constructor, m_monoObject, &param, nullptr);
		m_createMethod = mono_class_get_method_from_name(scriptClass, "OnCreate", 0);
		m_updateMethod = mono_class_get_method_from_name(scriptClass, "OnUpdate", 1);
        m_destroyMethod = mono_class_get_method_from_name(scriptClass, "OnDestroy", 0);

        void* iterator = nullptr;
        while (MonoClassField* field = mono_class_get_fields(scriptClass, &iterator))
        {
            uint32_t flags = mono_field_get_flags(field);

            if (flags & PUBLIC_FIELD_FLAG)
            {
                MonoType* type = mono_field_get_type(field);
                auto fieldType = MonoTypeToFieldType(type);
                Field scriptField;
                scriptField.Name = mono_field_get_name(field);
                scriptField.Type = fieldType;
                scriptField.ClassField = field;
                scriptField.ScriptName = m_className;
                m_fields.insert({ scriptField.Name,scriptField });
            }
        }
	}

    void ScriptInstance::GetFieldValue(const std::string& fieldName, void* buffer)const
    {
        auto it = m_fields.find(fieldName);
        if (it == m_fields.end())
            return;

        const Field& field = it->second;
        mono_field_get_value(m_monoObject, field.ClassField, buffer);
    }

    void ScriptInstance::SetFieldValue(const std::string& fieldName, void* buffer)const
    {

        auto it = m_fields.find(fieldName);
        if (it == m_fields.end())
            return;

        const Field& field = it->second;
        mono_field_set_value(m_monoObject, field.ClassField, buffer);
    }

    const std::unordered_map<std::string, Field>& ScriptInstance::GetFields()const
    {
        return m_fields;
    }

    const FieldType ScriptInstance::MonoTypeToFieldType(MonoType* type)const
    {
        std::string typeName = mono_type_get_name(type);
        auto itr = s_fieldTypeMap.find(typeName);
        if (itr != s_fieldTypeMap.end())
        {
            return itr->second;
        }
        return FieldType::Unknown;
    }

    const char* ScriptInstance::FieldTypeToString(FieldType type)const
    {
        switch (type)
        {
        case FieldType::Float: return "Float";
        case FieldType::Double: return "Double";
        case FieldType::Bool: return "Bool";
        case FieldType::Char: return "Char";
        case FieldType::Short: return "Short";
        case FieldType::Int: return "Int";
        case FieldType::Long: return "Long";
        case FieldType::Byte: return "Byte";
        case FieldType::UShort: return "UShort";
        case FieldType::UInt: return "UInt";
        case FieldType::ULong: return "ULong";
        case FieldType::Vector2: return "Vector2";
        case FieldType::Vector3: return "Vector3";
        case FieldType::Vector4: return "Vector4";
        case FieldType::Entity: return "Entity";
        case FieldType::TransformComponent: return "TransformComponent";
        case FieldType::RigidBody2DComponent: return "RigidBody2DComponent";
        default:return "Unknown";
        }
    }

    MonoObject* ScriptInstance::GetMonoObject()const
    {
        return m_monoObject;
    }

    MonoMethod* ScriptInstance::GetCreateMethod()const
    {
        return m_createMethod;
    }

    MonoMethod* ScriptInstance::GetUpdateMethod()const
    {
        return m_updateMethod;
    }

    MonoMethod* ScriptInstance::GetDestroyMethod()const
    {
        return m_destroyMethod;
    }

    ScriptInstance::~ScriptInstance()
    {
        mono_free_method(m_createMethod);
        mono_free_method(m_updateMethod);
    }
}