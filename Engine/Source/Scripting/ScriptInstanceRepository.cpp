#include "ScriptInstanceRepository.h"
#include "ScriptEngine.h"

namespace Chilli {
	std::unordered_map<uint64_t, std::shared_ptr<ScriptInstance>> ScriptInstanceRepository::s_scriptInstances;

	void ScriptInstanceRepository::MakeScriptInstance(const std::string& scriptName, uint64_t entityId)
	{
		s_scriptInstances.emplace(entityId,std::make_shared<ScriptInstance>(ScriptEngine::GetScriptByName(scriptName)->GetMonoClass(),entityId));
	}

	void ScriptInstanceRepository::RemoveScriptInstance(uint64_t entityId)
	{
		s_scriptInstances.erase(entityId);
	}

	std::shared_ptr<ScriptInstance> ScriptInstanceRepository::GetScriptInstanceByEntityId(uint64_t entityId)
	{
		if (auto scriptInst = s_scriptInstances.find(entityId); scriptInst != s_scriptInstances.end())
		{
			return scriptInst->second;
		}
		return nullptr;
	}
}