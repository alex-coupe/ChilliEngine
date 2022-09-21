#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"

namespace Engine::Scripting {
	class CHILLI_API ScriptingEngine : public Engine::Core::SubSystem {
	public:
		ScriptingEngine();
		virtual int GetSystemType()const override;
		virtual void ProcessFrame()override;
		static void TestPrint(MonoString* string);
	private:
		MonoDomain* m_domain = nullptr;
		MonoDomain* m_appDomain = nullptr;
		std::vector<MonoAssembly*> m_assemblies;
	};
}