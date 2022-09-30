#include "ScriptEngine.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
    ScriptEngine::ScriptEngine()
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

        m_coreAssemblyImage = mono_assembly_get_image(m_chilliCoreAssembly);
        MonoClass* coreClass = mono_class_from_name(m_coreAssemblyImage, "Chilli", "ChilliScript");
        m_constructor = mono_class_get_method_from_name(coreClass, ".ctor", 1);

        m_applicationScriptsImage = mono_assembly_get_image(m_applicationScriptsAssembly);
    }

    MonoImage* ScriptEngine::GetCoreAssemblyImage()const
    {
        return m_coreAssemblyImage;
    }

    void ScriptEngine::ConstructAndInvokeCreateMethod()const
    {
        auto currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
        for (const auto& entity : currentScene->GetEntities())
        {
            if (entity->HasComponent(ComponentTypes::Script))
            {
                auto scriptComp = std::static_pointer_cast<ScriptComponent>(entity->GetComponentByType(ComponentTypes::Script));
                auto script = DependencyResolver::ResolveDependency<ProjectManager>()->GetScriptByName(scriptComp->GetScriptName());
                uint64_t uuid = entity->Uuid.Get();
                void* param = &uuid;
                mono_runtime_invoke(m_constructor, script->GetMonoObject(), &param, nullptr);
                mono_runtime_invoke(script->GetCreateMethod(), script->GetMonoObject(), nullptr, nullptr);
            }
        }
    }

    void ScriptEngine::InvokeUpdateMethod() const
    {
        auto currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
        for (const auto& entity : currentScene->GetEntities())
        {
            if (entity->HasComponent(ComponentTypes::Script))
            {
                float dt = DependencyResolver().ResolveDependency<Timer>()->GetDeltaTime();
                void* deltaTime = &dt;
                auto scriptComp = std::static_pointer_cast<ScriptComponent>(entity->GetComponentByType(ComponentTypes::Script));
                auto script = DependencyResolver::ResolveDependency<ProjectManager>()->GetScriptByName(scriptComp->GetScriptName());
                mono_runtime_invoke(script->GetUpdateMethod(), script->GetMonoObject(), &deltaTime, nullptr);
            }
        }
    }

    std::vector<std::shared_ptr<Script>>  ScriptEngine::BuildAvailableScripts()
    {
        std::vector<std::shared_ptr<Script>> scripts;
        MonoImage* image = mono_assembly_get_image(m_applicationScriptsAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 1; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            scripts.emplace_back(std::make_shared<Script>(name, m_applicationScriptsImage, m_appDomain));
        }
        return scripts;
    }

    ScriptEngine::~ScriptEngine()
    {
        mono_domain_unload(m_appDomain);
        mono_jit_cleanup(m_domain);
    }
}