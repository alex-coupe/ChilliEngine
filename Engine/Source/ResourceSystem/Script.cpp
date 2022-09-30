#include "Script.h"

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
