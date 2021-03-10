#pragma once
#include "ChilliDefinitions.h"
#include "DependencyResolver.h"
#pragma warning(disable:4251)
namespace Engine::Core {

	enum class SystemTypes {
		Timer = 1, EventSystem = 2, Renderer = 3
	};

	class CHILLI_API EngineSystem {
	public:
		EngineSystem(std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver) : m_resolver(m_resolver)
		{}
		virtual int GetHash() = 0;		
		virtual ~EngineSystem() {};
	protected:
		std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver;
	};
	
}