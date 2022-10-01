#pragma once
#include "../Core/ChilliDefinitions.h"
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"
#include "Script.h"
#include "ScriptApi.h"

namespace Chilli {
	class CHILLI_API ScriptEngine {
	public:
		static void Init();
		static void InvokeCreateMethod();
		static void InvokeUpdateMethod();
		static void InvokeDestroyMethod();
		static void Shutdown();
		static MonoDomain* GetAppDomain();
		static MonoImage* GetCoreAssemblyImage();
		static std::shared_ptr<Script> GetScriptByName(const std::string& name);
		static std::vector<std::shared_ptr<Script>> GetAvailableScripts();
	private:
		static void BuildAvailableScripts();
		static MonoDomain* s_domain;
		static MonoDomain* s_appDomain;
		static MonoImage* s_coreAssemblyImage;
		static MonoImage* s_applicationScriptsImage;
		static MonoAssembly* s_chilliCoreAssembly;
		static MonoAssembly* s_applicationScriptsAssembly;
		static std::vector<std::shared_ptr<Script>> s_scripts;
	};
}