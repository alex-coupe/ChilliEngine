#pragma once
#include "../Core/ChilliDefinitions.h"
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"

namespace Chilli {

	class CHILLI_API Script {
	public:
		Script(const std::string& className, MonoImage* image, MonoDomain* appDomain);
		const std::string& GetScriptName()const;
		MonoClass* GetMonoClass()const;
	private:
		MonoClass* m_monoClass = nullptr;
		std::string m_className;
	};
}