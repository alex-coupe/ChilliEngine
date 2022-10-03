#include "ScriptEngine.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {

    MonoDomain* ScriptEngine::s_domain = nullptr;
    MonoDomain* ScriptEngine::s_appDomain = nullptr;
    MonoImage* ScriptEngine::s_coreAssemblyImage = nullptr;
    MonoImage* ScriptEngine::s_applicationScriptsImage = nullptr;
    MonoAssembly* ScriptEngine::s_chilliCoreAssembly = nullptr;
    MonoAssembly* ScriptEngine::s_applicationScriptsAssembly = nullptr;
    std::vector<std::shared_ptr<Script>> ScriptEngine::s_scripts;

    void ScriptEngine::Init()
    {
        mono_set_assemblies_path("..\\Dependencies\\Mono\\lib");

        s_domain = mono_jit_init("ChilliJITRuntime");

        if (s_domain == nullptr)
        {
            CHILLI_ERROR("Script engine domain not initialised!");
            return;
        }

        s_appDomain = mono_domain_create_appdomain(nullptr, nullptr);

        s_chilliCoreAssembly = mono_domain_assembly_open(s_domain, "Assets\\Scripts\\bin\\ChilliScriptCore.dll");
        s_applicationScriptsAssembly = mono_domain_assembly_open(s_domain, "Assets\\Scripts\\bin\\ApplicationScripts.dll");
        s_coreAssemblyImage = mono_assembly_get_image(s_chilliCoreAssembly);      
        s_applicationScriptsImage = mono_assembly_get_image(s_applicationScriptsAssembly);
        BuildAvailableScripts();
    }

    MonoDomain* ScriptEngine::GetAppDomain()
    {
        return s_appDomain;
    }

    MonoImage* ScriptEngine::GetCoreAssemblyImage()
    {
        return s_coreAssemblyImage;
    }

    std::shared_ptr<Script> ScriptEngine::GetScriptByName(const std::string& name)
    {
        if (auto scriptItr = std::find_if(s_scripts.begin(), s_scripts.end(), [&name](const std::shared_ptr<Script> rhs)
            {
                return rhs->GetScriptName() == name;
            }); scriptItr != s_scripts.end())
        {
            return *scriptItr;
        }
            return nullptr;
    }

    void ScriptEngine::InvokeCreateMethod()
    {
        auto currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
        for (const auto& entity : currentScene->GetEntities())
        {
            if (entity->HasComponent(ComponentType::Script))
            {
                const auto& scriptInstance = ScriptInstanceRepository::GetScriptInstanceByEntityId(entity->Uuid.Get());
                mono_runtime_invoke(scriptInstance->GetCreateMethod(), scriptInstance->GetMonoObject(), nullptr, nullptr);
            }
        }
    }

    void ScriptEngine::InvokeUpdateMethod()
    {
        auto currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
        for (const auto& entity : currentScene->GetEntities())
        {
            if (entity->HasComponent(ComponentType::Script))
            {
                float dt = DependencyResolver().ResolveDependency<Timer>()->GetDeltaTime();
                void* deltaTime = &dt;
                const auto& scriptInstance = ScriptInstanceRepository::GetScriptInstanceByEntityId(entity->Uuid.Get());
                mono_runtime_invoke(scriptInstance->GetUpdateMethod(), scriptInstance->GetMonoObject(), &deltaTime, nullptr);
            }
        }
    }

    std::vector<std::shared_ptr<Script>> ScriptEngine::GetAvailableScripts()
    {
        return s_scripts;
    }

    void ScriptEngine::InvokeDestroyMethod()
    {
        auto currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
        for (const auto& entity : currentScene->GetEntities())
        {
            if (entity->HasComponent(ComponentType::Script))
            {
                const auto& scriptInstance = ScriptInstanceRepository::GetScriptInstanceByEntityId(entity->Uuid.Get());
                mono_runtime_invoke(scriptInstance->GetDestroyMethod(), scriptInstance->GetMonoObject(), nullptr, nullptr);
            }
        }
    }

    void ScriptEngine::BuildAvailableScripts()
    {
        MonoImage* image = mono_assembly_get_image(s_applicationScriptsAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 1; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            s_scripts.emplace_back(std::make_shared<Script>(name, s_applicationScriptsImage, s_appDomain));
        }
    }

    void ScriptEngine::Shutdown()
    {
        mono_domain_unload(s_appDomain);
        mono_jit_cleanup(s_domain);
    }
}