#pragma once
#include "ChilliDefinitions.h"
#include "DependencyResolver.h"
#pragma warning(disable:4251)
namespace Engine::Core {

	enum class SystemTypes {
		Timer = 1, EventSystem = 2, Renderer = 3, Window = 4
	};

	class CHILLI_API EngineSystem {
	public:
		EngineSystem(std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver) : m_resolver(m_resolver)
		{}
		virtual int GetHash() = 0;		
		virtual ~EngineSystem() {};
		virtual void ProcessFrame() = 0;
	protected:
		std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver;
	};
	
}