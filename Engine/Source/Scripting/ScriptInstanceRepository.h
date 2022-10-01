#pragma once
#include "../Core/ChilliDefinitions.h"
#include "./ScriptInstance.h"

namespace Chilli {

	class CHILLI_API ScriptInstanceRepository {
	public:
		static std::shared_ptr<ScriptInstance> MakeScriptInstance(const std::string& scriptName, uint64_t entityId);
		static void RemoveScriptInstance(uint64_t entityId);
		static std::shared_ptr<ScriptInstance> GetScriptInstanceByEntityId(uint64_t);
	private:
		static std::unordered_map<uint64_t, std::shared_ptr<ScriptInstance>> s_scriptInstances;
	};
}