#pragma once
#include "ChilliDefinitions.h"
#pragma warning(disable:4251)
namespace Chilli {

	enum class SystemType {
		Timer = 1, Events = 2, Renderer = 3, ProjectManager = 4
	};

	class CHILLI_API SubSystem {
	public:
		SubSystem()
		{}
		virtual ~SubSystem() {};
		virtual void ProcessFrame() = 0;
	};
	
}