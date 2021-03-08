#pragma once
#include "ChilliDefinitions.h"
#include <map>
#include <string>
#include <sstream>


namespace Engine::Core {

	enum class EventType : unsigned {
		None = 0, FocusGained = 7, FocusLost = 8, MouseMove = 512, LeftMousePressed = 513, LeftMouseReleased = 514,
		RightMousePressed = 516, RightMouseReleased = 517, MouseWheel = 522

	};

	EventType operator |(EventType lhs, EventType rhs);
	EventType operator &(EventType lhs, EventType rhs);
	EventType operator ^(EventType lhs, EventType rhs);
	EventType operator ~(EventType rhs);
	EventType& operator |=(EventType& lhs, EventType rhs);
	EventType& operator &=(EventType& lhs, EventType rhs);

	struct CHILLI_API  EventData {
		uint64_t wparam = 0;
		int64_t lparam = 0;
		uint32_t msg = 0;
		int64_t m_mouseX = 0;
		int64_t m_mouseY = 0;
	};

	class CHILLI_API Event{
	public:
		Event(EventData& data_in);
		EventType GetType() const;
		EventData m_data;
	};



}