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

    auto assembly = mono_domain_assembly_open(m_domain, "Assets\\Scripts\\ChilliScriptCore.dll");
    m_assemblies.push_back(assembly);

    //Metadata
    PrintAssemblyTypes(assembly);
    //Class constructor
    MonoImage* image = mono_assembly_get_image(assembly);
    MonoClass* cls = mono_class_from_name(image, "ChilliEngine", "Main");
    MonoObject* obj = mono_object_new(m_appDomain, cls);
    mono_runtime_object_init(obj);
    //Call method no params
    MonoMethod* method = mono_class_get_method_from_name(cls, "PrintMessage", 0);
    mono_runtime_invoke(method, obj, nullptr, nullptr);
    //Call method 1 param
    MonoMethod* methodTwo = mono_class_get_method_from_name(cls, "PrintInt", 1);
    int value = 5;
    void* param = &value;
    mono_runtime_invoke(methodTwo, obj, &param, nullptr);
    int valueTwo = 10;
    void* param2 = &valueTwo;
    void* params[2] = {
        param,param2
    };
    //Call method multiple params
    MonoMethod* methodThree = mono_class_get_method_from_name(cls, "Add", 2);
    mono_runtime_invoke(methodThree, obj, params, nullptr);
    //Call method with string param
    MonoString* string = mono_string_new(m_appDomain, "Hello World from C++");
    MonoMethod* methodFour = mono_class_get_method_from_name(cls, "PrintCustomMessage", 1);
    void* stringParam = string;
    mono_runtime_invoke(methodFour, obj, &stringParam, nullptr);
}

void Engine::Scripting::ScriptEngine::LoadAssembly(Engine::Utilities::UUID& uuid)
{
        auto script = std::static_pointer_cast<Engine::ResourceSystem::Script>(Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAssetByUUID(uuid));
       
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(script->GetPayload(), script->GetPayloadSize(), 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            CHILLI_ERROR(errorMessage);
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, script->GetFilePath().string().c_str(), &status, 0);
        mono_image_close(image);

        PrintAssemblyTypes(assembly);

        m_assemblies.push_back(std::move(assembly));
}

void Engine::Scripting::ScriptEngine::RunScripts() const
{
}

void Engine::Scripting::ScriptEngine::PrintAssemblyTypes(MonoAssembly* assembly)
{
    MonoImage* image = mono_assembly_get_image(assembly);
    const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

    for (int32_t i = 0; i < numTypes; i++)
    {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
        const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

        printf("%s.%s\n", nameSpace, name);
    }
}

void Engine::Scripting::ScriptEngine::TestPrint(MonoString* string)
{
    char* cppString = mono_string_to_utf8(string);

    CHILLI_INFO(cppString);

    mono_free(cppString);
}

Engine::Scripting::ScriptEngine::~ScriptEngine()
{
    mono_domain_unload(m_appDomain);
    mono_jit_cleanup(m_domain);
}
