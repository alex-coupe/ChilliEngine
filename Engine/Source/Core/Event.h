#pragma once
#include "ChilliDefinitions.h"
#include <map>
#include <string>
#include <sstream>
#include "windows.h"
#pragma warning(disable:4251)
namespace Engine::Core {

#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))

	enum class EventCategory {
		None,KeyboardEvent, MouseEvent, SystemEvent
	};

	static std::map<uint32_t, std::string> EventType = {
			{6, "Window Activated"},
			{8, "Window Lost Focus"},
			{512,"Mouse Move"},
			{513,"Left Mouse Button Pressed"},
			{514,"Left Mouse Button Released"},
			{516,"Right Mouse Button Pressed"},
			{517,"Right Mouse Button Released"},
			{522, "Mouse Wheel"}

	};
	
	struct CHILLI_FUNC  EventData {
		uint64_t wparam = 0;
		int64_t lparam = 0;
		uint32_t msg = 0;
	};

	CHILLI_CLASS Event{
	public:
		EventCategory GetCategory() { return m_category; }
		virtual const std::string ToString() const = 0;
	protected:
		EventCategory m_category = EventCategory::None;
		int64_t m_lparam = 0;
		uint64_t m_wparam = 0;
		uint32_t msg = 0;
	};

	CHILLI_CLASS KeyboardEvent : public Event{

	public:
		KeyboardEvent(EventData& data) 
		{ 
			msg = data.msg;
			m_lparam = data.lparam; 
			m_wparam = data.wparam; 
			m_category = EventCategory::KeyboardEvent; 
		}
		const std::string ToString()const override 
		{ 
			return EventType.find(msg) != EventType.end() ? EventType.at(msg) : ""; 
		}
	};

	CHILLI_CLASS MouseEvent : public Event{

	public:
		MouseEvent(EventData& data) 
		{
			msg = data.msg;
			m_lparam = data.lparam;
			m_wparam = data.wparam;
			m_category = EventCategory::MouseEvent;
		}
		const std::string ToString()const override
		{ 
			std::stringstream ss(EventType.find(msg) != EventType.end() ? EventType.at(msg) : "Unknown Event");

			if (msg == 512)
			{
				auto x = m_lparam & 0x0000FFFF;
				auto y = m_lparam >> 16;
				ss << " x: " << x << " y: " << y;
			}

			if (msg == 522)
			{

				auto delta = GET_WHEEL_DELTA_WPARAM(m_wparam);

					ss << "delta: " << delta;
			}
			return ss.str();
		}
	};

	CHILLI_CLASS SystemEvent : public Event{

	public:
		SystemEvent(EventData& data)  
		{
			msg = data.msg;
			m_lparam = data.lparam;
			m_wparam = data.wparam;
			m_category = EventCategory::SystemEvent;
		}
		const std::string ToString()const override
		{ 
			return EventType.find(msg) != EventType.end() ? EventType.at(msg) : ""; 
		}
	};

}