#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Asset.h"
#include <string>
#include <fstream>
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"

namespace Chilli {
	class CHILLI_API Script : public Asset {
	public:
		Script(const std::string& className, UUID uuid, MonoImage* image, MonoDomain* appDomain);
		Script(const std::string& className, MonoImage* image, MonoDomain* appDomain);
		virtual const std::string Serialize()const override;
		const std::string& GetScriptName()const;
		MonoObject* GetMonoObject()const;
		MonoMethod* GetCreateMethod()const;
		MonoMethod* GetUpdateMethod()const;
		~Script();
	private:
		MonoClass* m_monoClass = nullptr;
		MonoObject* m_monoObject = nullptr;
		MonoMethod* m_createMethod = nullptr;
		MonoMethod* m_updateMethod = nullptr;
		std::string m_className;
	};
}