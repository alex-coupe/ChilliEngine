#pragma once
#include "ChilliDefinitions.h"
#include "Event.h"
#include <queue>
#include <functional>
#pragma warning(disable:4251)
namespace Engine::Core {

	struct CHILLI_API EventSystemData {
		int64_t mouse_x = 0;
		int64_t mouse_y = 0;
		int64_t screen_width = 0;
		int64_t screen_height = 0;
	};
	
	class CHILLI_API EventSystem{
	using tuple = std::vector<std::pair<std::vector<EventType>,std::function<void()>>>;
	public:
		EventSystem() = default;
		void ClearBuffer();
		void ProcessFrame();
		void TrimBuffer();
		void Push(Event* e);
		void Subscribe(std::vector<EventType> types, std::function<void()> callback);
		void TranslateEvent(EventType type, const EventData& data_in);
		EventSystemData m_data;
	private:
		std::queue<Event*> m_eventBuffer;
		tuple m_Subscribers;
	};


}