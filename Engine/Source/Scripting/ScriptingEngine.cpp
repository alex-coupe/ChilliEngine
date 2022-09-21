#include "ScriptingEngine.h"

Engine::Scripting::ScriptingEngine::ScriptingEngine()
{
    mono_set_assemblies_path("C:\\Dev\\ChilliEngine\\Dependencies\\Mono\\lib");

    m_domain = mono_jit_init("ScriptEngineDomain");

    if (m_domain == nullptr)
    {
        CHILLI_ERROR("Script engine domain not initialised!");
        return;
    }
    
    m_appDomain = mono_domain_create_appdomain(nullptr, nullptr);

    auto assembly = mono_domain_assembly_open(m_domain, "C:\\Dev\\ChilliEngine\\Application\\Assets\\Scripts\\CSharpApp.exe");
    m_assemblies.push_back(assembly);

    mono_add_internal_call("CSharpApp.Program::PrintMethod", Engine::Scripting::ScriptingEngine::TestPrint);

    int argc = 1;
    char* argv[1] = { (char*)"CSharp" };

  
    mono_jit_exec(m_domain, assembly, argc, argv);
}

int Engine::Scripting::ScriptingEngine::GetSystemType() const
{
    return (int)Engine::Core::SystemTypes::ScriptingEngine;
}

void Engine::Scripting::ScriptingEngine::ProcessFrame()
{
}

void Engine::Scripting::ScriptingEngine::TestPrint(MonoString* string)
{
    char* cppString = mono_string_to_utf8(string);

    CHILLI_INFO(cppString);

    mono_free(cppString);
}
