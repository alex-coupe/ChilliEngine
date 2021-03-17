#pragma once
#include "ChilliDefinitions.h"
#include "DependencyResolver.h"
#pragma warning(disable:4251)
namespace Engine::Core {

	enum class SystemTypes {
		Timer = 1, EventSystem = 2, Renderer = 3, Window = 4, SceneManager = 5
	};

	class CHILLI_API SubSystem {
	public:
		SubSystem(const std::shared_ptr<DependencyResolver<SubSystem>>& m_resolver) : m_resolver(m_resolver)
		{}
		virtual int GetHash() const = 0;		
		virtual ~SubSystem() {};
		virtual void ProcessFrame() = 0;
	protected:
		std::shared_ptr<DependencyResolver<SubSystem>> m_resolver;
	};
	
}