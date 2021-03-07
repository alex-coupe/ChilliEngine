#pragma once
#include "ChilliDefinitions.h"
#include "Event.h"
#include <queue>
#include <map>
#pragma warning(disable:4251)
namespace Engine::Core {

	CHILLI_CLASS EventSystem{
	
	public:
		EventSystem() = default;
		void ClearBuffer();
		void ProcessFrame();
		void Push(Event* e);
		size_t Subscribe(EventCategory category, void* callback);
		void Unsubscribe(EventCategory category, size_t identifier);
	private:
		std::queue<Event*> m_eventBuffer;
		std::map<size_t, void*> m_keyboardSubscribers;
		std::map<size_t, void*> m_mouseSubscribers;
		std::map<size_t, void*> m_systemSubscribers;
	};
}