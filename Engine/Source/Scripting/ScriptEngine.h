#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"
#include <filesystem>
#include "../Core/UUID.h"
#include "../Core/DependencyResolver.h"
#include "../ResourceSystem/Script.h"
#include "ScriptApi.h"

namespace Chilli {
	class CHILLI_API ScriptEngine {
	public:
		ScriptEngine();
		void InvokeCreateMethod()const;
		void InvokeUpdateMethod()const;
		std::vector<std::shared_ptr<Script>> BuildAvailableScripts();
		~ScriptEngine();
		
	private:
		
		
		MonoDomain* m_domain = nullptr;
		MonoDomain* m_appDomain = nullptr;
		MonoImage* m_applicationScriptsImage = nullptr;
		MonoAssembly* m_chilliCoreAssembly = nullptr;
		MonoAssembly* m_applicationScriptsAssembly = nullptr;
	};
}