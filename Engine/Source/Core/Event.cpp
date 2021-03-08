#include "Event.h"
#include <Windows.h>
namespace Engine::Core {


	EventType operator |(EventType lhs, EventType rhs)
	{
		return static_cast<EventType> (
			static_cast<std::underlying_type<EventType>::type>(lhs) |
			static_cast<std::underlying_type<EventType>::type>(rhs)
			);
	}

	EventType operator &(EventType lhs, EventType rhs)
	{
		return static_cast<EventType> (
			static_cast<std::underlying_type<EventType>::type>(lhs) &
			static_cast<std::underlying_type<EventType>::type>(rhs)
			);
	}

	EventType operator ^(EventType lhs, EventType rhs)
	{
		return static_cast<EventType> (
			static_cast<std::underlying_type<EventType>::type>(lhs) ^
			static_cast<std::underlying_type<EventType>::type>(rhs)
			);
	}

	EventType operator ~(EventType rhs)
	{
		return static_cast<EventType> (
			~static_cast<std::underlying_type<EventType>::type>(rhs)
			);
	}

	EventType& operator |=(EventType& lhs, EventType rhs)
	{
		lhs = static_cast<EventType> (
			static_cast<std::underlying_type<EventType>::type>(lhs) |
			static_cast<std::underlying_type<EventType>::type>(rhs)
			);

		return lhs;
	}

	EventType& operator &=(EventType& lhs, EventType rhs)
	{
		lhs = static_cast<EventType> (
			static_cast<std::underlying_type<EventType>::type>(lhs) &
			static_cast<std::underlying_type<EventType>::type>(rhs)
			);

		return lhs;
	}

	Event::Event(EventData& data_in)
		: m_data(data_in)
	{}

	EventType Event::GetType() const
	{
		return static_cast<EventType>(m_data.msg);
	}
	
}
