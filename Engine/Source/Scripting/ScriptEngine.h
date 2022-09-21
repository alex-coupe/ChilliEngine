#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"
#include <filesystem>
#include "../Utilities/UUID.h"
#include "../Core/DependencyResolver.h"
#include "../ResourceSystem/Script.h"

namespace Engine::Scripting {
	class CHILLI_API ScriptEngine {
	public:
		ScriptEngine();
		void LoadAssembly(Engine::Utilities::UUID& uuid);
		void RunScripts()const;
		void PrintAssemblyTypes(MonoAssembly* assembly);
		static void TestPrint(MonoString* string);
		~ScriptEngine();
	private:
		MonoDomain* m_domain = nullptr;
		MonoDomain* m_appDomain = nullptr;
		std::vector<MonoAssembly*> m_assemblies;
	};
}