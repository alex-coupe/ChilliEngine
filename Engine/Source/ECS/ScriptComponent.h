#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../ECS/Component.h"
#include "../Scripting/Script.h"

namespace Chilli {
	class CHILLI_API ScriptComponent :public Component {
	public:
		ScriptComponent();
		ScriptComponent(const std::string& scriptName);
		ScriptComponent(const ScriptComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		const std::string& GetScriptName()const;
		void SetScript(const std::string& scriptName);
	private:
		std::string m_scriptName;
	};
}