#pragma once
#include "ChilliDefinitions.h"


namespace Engine::Core {

	enum class EventCategory {
		None,KeyboardEvent, MouseEvent, SystemEvent
	};

	CHILLI_CLASS Event{
	public:
		EventCategory GetType() { return m_category; }
		virtual const char* ToString() = 0;
	protected:
		EventCategory m_category = EventCategory::None;
	};

	CHILLI_CLASS KeyboardEvent : public Event{

	public:
		KeyboardEvent() { m_category = EventCategory::KeyboardEvent;}
		const char* ToString()override { return "Keyboard Event"; }
	private:
	};

	CHILLI_CLASS MouseEvent : public Event{

	public:
		MouseEvent(){m_category = EventCategory::MouseEvent;}
		const char* ToString()override { return "Mouse Event"; }
	private:
	};

	CHILLI_CLASS SystemEvent : public Event{

	public:
		SystemEvent(){m_category = EventCategory::SystemEvent;}
		const char* ToString()override { return "System Event"; }
	private:
		
	};

}