#pragma once
#include "ChilliDefinitions.h"
#include <map>
#include <string>
#include <sstream>
#include <Windows.h>

namespace Engine::Core {

	enum class EventType : unsigned {
		None = 0, WindowResized = 3, FocusGained = 7, FocusLost = 8,  KeyDown = 256, KeyUp = 257, Character = 258, SysKeyDown = 260, SysKeyUp = 261,  MouseMove = 512,
		LeftMousePressed = 513, LeftMouseReleased = 514,RightMousePressed = 516, RightMouseReleased = 517, MouseWheel = 522, 
		MouseLeave = 675

	};

	EventType operator |(EventType lhs, EventType rhs);
	EventType operator &(EventType lhs, EventType rhs);
	EventType operator ^(EventType lhs, EventType rhs);
	EventType operator ~(EventType rhs);
	EventType& operator |=(EventType& lhs, EventType rhs);
	EventType& operator &=(EventType& lhs, EventType rhs);

	struct EventData {
		uint64_t wparam = 0;
		int64_t lparam = 0;
		uint32_t msg = 0;
		HWND* handle = nullptr;
	};

	class CHILLI_API Event{
	public:
		Event(EventData& data_in);
		EventType GetType() const;
		EventData m_data;
	};



}