#include "Script.h"

namespace Chilli {

    Script::Script(const std::string& className, MonoImage* image, MonoDomain* appDomain)
        : m_className(className), m_monoClass(mono_class_from_name(image, "Application", className.c_str()))
    {}   

    const std::string& Script::GetScriptName()const
    {
        return m_className;
    } 

    MonoClass* Script::GetMonoClass()const
    {
        return m_monoClass;
    }
}
