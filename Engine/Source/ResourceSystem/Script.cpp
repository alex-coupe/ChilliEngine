#include "Script.h"
#define PUBLIC_FIELD_FLAG 6
#define PRIVATE_FIELD_FLAG 1

namespace Chilli {

    Script::Script(const std::string& className,UUID uuid, MonoImage* image, MonoDomain* appDomain)
        : Asset(AssetType::Script, "Assets/Scripts", uuid), m_className(className)
    {
        m_monoClass = mono_class_from_name(image, "Application", className.c_str());
        m_monoObject = mono_object_new(appDomain, m_monoClass);
        mono_runtime_object_init(m_monoObject);
        m_createMethod = mono_class_get_method_from_name(m_monoClass, "OnCreate", 0);
        m_updateMethod = mono_class_get_method_from_name(m_monoClass, "OnUpdate", 1);
    }

    Script::Script(const std::string& className, MonoImage* image, MonoDomain* appDomain)
        : Asset(AssetType::Script, "Assets/Scripts", UUID()), m_className(className)
    {
        m_monoClass = mono_class_from_name(image, "Application", className.c_str());
        m_monoObject = mono_object_new(appDomain, m_monoClass);
        mono_runtime_object_init(m_monoObject);
        m_createMethod = mono_class_get_method_from_name(m_monoClass, "OnCreate", 0);
        m_updateMethod = mono_class_get_method_from_name(m_monoClass, "OnUpdate", 1);

        void* iterator = nullptr;
        while (MonoClassField* field = mono_class_get_fields(m_monoClass, &iterator))
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

    void Script::GetFieldValue(const std::string& fieldName, void* buffer)const
    {
        auto it = m_fields.find(fieldName);
        if (it == m_fields.end())
            return;

        const Field& field = it->second;
        mono_field_get_value(m_monoObject, field.ClassField, buffer);
    }

    void Script::SetFieldValue(const std::string& fieldName, void* buffer)const
    {

        auto it = m_fields.find(fieldName);
        if (it == m_fields.end())
            return;

        const Field& field = it->second;
        mono_field_set_value(m_monoObject, field.ClassField, buffer);
    }

    const std::unordered_map<std::string, Field>& Script::GetFields()const
    {
        return m_fields;
    }

    const FieldType Script::MonoTypeToFieldType(MonoType* type)const
    {
        std::string typeName = mono_type_get_name(type);
        auto itr = s_fieldTypeMap.find(typeName);
        if (itr != s_fieldTypeMap.end())
        {
            return itr->second;
        }
        return FieldType::Unknown;
    }

    const char* Script::FieldTypeToString(FieldType type)const
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

    const std::string& Script::GetScriptName()const
    {
        return m_className;
    }
    
    const std::string Script::Serialize() const
    {
        std::stringstream ss;
        ss << "{ \"Uuid\":" << Uuid.Get() << ", \"Type\":" << static_cast<int>(m_type) << ", \"FilePath\": \"Assets/Scripts/" << m_filePath.filename().string() << "\"}";
        return  ss.str();
    }

    MonoObject* Script::GetMonoObject()const
    {
        return m_monoObject;
    }

    MonoMethod* Script::GetCreateMethod()const
    {
        return m_createMethod;
    }

    MonoMethod* Script::GetUpdateMethod()const
    {
        return m_updateMethod;
    }

    Script::~Script()
    {
        mono_free_method(m_createMethod);
        mono_free_method(m_updateMethod);
    }

    
}
