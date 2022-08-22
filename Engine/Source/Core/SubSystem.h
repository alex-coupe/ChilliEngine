#pragma once
#include "ChilliDefinitions.h"
#pragma warning(disable:4251)
namespace Engine::Core {

	enum class SystemTypes {
		Timer = 1, EventSystem = 2, Renderer = 3, Window = 4, SceneManager = 5
	};

	class CHILLI_API SubSystem {
	public:
		SubSystem()
		{}
		virtual int GetSystemType() const = 0;		
		virtual ~SubSystem() {};
		virtual void ProcessFrame() = 0;
	};
	
}