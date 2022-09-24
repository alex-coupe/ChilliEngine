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
		void SceneStart()const;
		void SceneUpdate()const;
		const std::vector<std::string>& GetAvailableClasses()const;
		~ScriptEngine();
	private:
		void BuildAvailableClasses();
		std::vector<std::string> m_availableClasses;
		MonoDomain* m_domain = nullptr;
		MonoDomain* m_appDomain = nullptr;
		MonoImage* m_applicationScriptsImage = nullptr;
		MonoAssembly* m_chilliCoreAssembly = nullptr;
		MonoAssembly* m_applicationScriptsAssembly = nullptr;
	};
}