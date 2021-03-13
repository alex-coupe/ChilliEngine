#pragma once
#include "ChilliDefinitions.h"
#include <queue>
#include <functional>
#include "EngineSystem.h"
#include <Windows.h>
#pragma warning(disable:4251)
namespace Engine::Core {

	struct EventData {
		uint64_t wparam = 0;
		int64_t lparam = 0;
		uint32_t msg = 0;
		HWND handle;
	};

	enum class EventType : uint32_t {
		None = 0, WindowResized = 5, FocusGained = 7, FocusLost = 8, Close = 16, KeyDown = 256, KeyUp = 257, Character = 258, SysKeyDown = 260, SysKeyUp = 261, MouseMove = 512,
		LeftMousePressed = 513, LeftMouseReleased = 514, RightMousePressed = 516, RightMouseReleased = 517, MouseWheel = 522,
		MouseLeave = 675
	};

	EventType operator |(EventType lhs, EventType rhs);
	EventType operator &(EventType lhs, EventType rhs);
	EventType operator ^(EventType lhs, EventType rhs);
	EventType operator ~(EventType rhs);
	EventType& operator |=(EventType& lhs, EventType rhs);
	EventType& operator &=(EventType& lhs, EventType rhs);
	
	class CHILLI_API EventSystem : public EngineSystem { 
	public:
		EventSystem(const std::shared_ptr<DependencyResolver<EngineSystem>>& m_resolver);
		~EventSystem();
		EventSystem(const EventSystem&) = delete;
		EventSystem& operator=(const EventSystem&) = delete;
		EventSystem(EventSystem&&)noexcept = default;
		int GetHash()const override;
		void ClearBuffer();
		void ProcessFrame()override;
		void TrimBuffer();
		void Push(const EventData*const e);
		void Subscribe(const std::vector<EventType>& types, std::function<void()> callback);
		void TranslateEvent(const EventData* const data_in);
		int64_t& GetMouseX();
		int64_t& GetMouseY();
		unsigned char& GetKeyCode();
		int64_t& GetScreenWidth();
		int64_t& GetScreenHeight();
		short& GetWheelDelta();
	private:
		int64_t m_screenWidth = 0;
		int64_t m_screenHeight = 0;
		int64_t m_mouseX = 0;
		int64_t m_mouseY = 0;
		unsigned char m_keycode = 0;
		short m_wheelDelta = 0;
		std::queue<const EventData*> m_eventBuffer;
		std::vector<std::pair<std::vector<EventType>, std::function<void()>>> m_Subscribers;
	};


}