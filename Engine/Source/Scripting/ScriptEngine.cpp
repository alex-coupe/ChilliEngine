#include "ScriptEngine.h"
#include "../ResourceSystem/ProjectManager.h"

Engine::Scripting::ScriptEngine::ScriptEngine()
{
    mono_set_assemblies_path("..\\Dependencies\\Mono\\lib");

    m_domain = mono_jit_init("ChilliJITRuntime");

    if (m_domain == nullptr)
    {
        CHILLI_ERROR("Script engine domain not initialised!");
        return;
    }

    m_appDomain = mono_domain_create_appdomain(nullptr, nullptr);

    m_chilliCoreAssembly = mono_domain_assembly_open(m_domain, "Assets\\Scripts\\bin\\ChilliScriptCore.dll");
    m_applicationScriptsAssembly = mono_domain_assembly_open(m_domain, "Assets\\Scripts\\bin\\ApplicationScripts.dll");

    ScriptApi::Init();

    m_applicationScriptsImage = mono_assembly_get_image(m_applicationScriptsAssembly);
    BuildAvailableClasses();
}

void Engine::Scripting::ScriptEngine::SceneStart()const
{
    auto currentScene = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetCurrentScene();
    for (const auto& entity : currentScene->GetEntities())
    {
        if (entity->HasComponent(Engine::ECS::ComponentTypes::Script))
        {
            auto scriptComp = std::static_pointer_cast<Engine::ECS::ScriptComponent>(entity->GetComponentByType(Engine::ECS::ComponentTypes::Script));
            auto monoClass = mono_class_from_name(m_applicationScriptsImage, "Application", scriptComp->GetScriptName().c_str());
            auto monoObject = mono_object_new(m_appDomain, monoClass);
            mono_runtime_object_init(monoObject);
            MonoMethod* method = mono_class_get_method_from_name(monoClass, "OnCreate", 0);
            mono_runtime_invoke(method, monoObject, nullptr, nullptr);
        }
    }
}

void Engine::Scripting::ScriptEngine::SceneUpdate() const
{
    auto currentScene = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetCurrentScene();
    for (const auto& entity : currentScene->GetEntities())
    {
        if (entity->HasComponent(Engine::ECS::ComponentTypes::Script))
        {
            auto scriptComp = std::static_pointer_cast<Engine::ECS::ScriptComponent>(entity->GetComponentByType(Engine::ECS::ComponentTypes::Script));
            auto monoClass = mono_class_from_name(m_applicationScriptsImage, "Application", scriptComp->GetScriptName().c_str());
            auto monoObject = mono_object_new(m_appDomain, monoClass);
            mono_runtime_object_init(monoObject);
            MonoMethod* methodTwo = mono_class_get_method_from_name(monoClass, "OnUpdate", 1);
            float dt = Engine::Core::DependencyResolver().ResolveDependency<Engine::Core::Timer>()->GetDeltaTime();
            void* deltaTime = &dt;
            mono_runtime_invoke(methodTwo, monoObject, &deltaTime, nullptr);
        }
    }
}

void Engine::Scripting::ScriptEngine::BuildAvailableClasses()
{
    MonoImage* image = mono_assembly_get_image(m_applicationScriptsAssembly);
    const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

    for (int32_t i = 1; i < numTypes; i++)
    {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

        m_availableClasses.push_back(name);
    }
}

const std::vector<std::string>& Engine::Scripting::ScriptEngine::GetAvailableClasses()const
{
    return m_availableClasses;
}

Engine::Scripting::ScriptEngine::~ScriptEngine()
{
    mono_domain_unload(m_appDomain);
    mono_jit_cleanup(m_domain);
}
