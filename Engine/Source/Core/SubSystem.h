#pragma once
#include "ChilliDefinitions.h"
#pragma warning(disable:4251)
namespace Engine::Core {

	enum class SystemTypes {
		Timer = 1, Events = 2, Renderer = 3, ProjectManager = 4, GuiManager = 5, Physics = 6,
		Scripting = 7, Animation = 8, Sound = 9, Networking = 10
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