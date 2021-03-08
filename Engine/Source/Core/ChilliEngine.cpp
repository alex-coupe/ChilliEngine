#include "ChilliEngine.h"


void MouseMovedCallback(int64_t& x, int64_t& y)
{
	ENGINE_INFO("Mouse Moved - x: {} y: {}", x, y);
}

void MouseClickedCallback()
{
	ENGINE_INFO("Mouse Clicked");
}

void FocusLostCallback()
{
	ENGINE_INFO("Window Focus Lost");
}

void FocusGainedCallback()
{
	ENGINE_INFO("Window Focus Gained");
}

void KeyboardCallback(unsigned char keycode)
{
	ENGINE_INFO("Key Press {}", keycode);
}

void ResizeCallback(int64_t& width, int64_t& height)
{
	ENGINE_INFO("Window Resize width: {} height: {}", width, height);
}

void ChilliEngine::Init()
{
	//Initialise SubSystems
	m_eventSystem = std::make_unique<EventSystem>();

	//Subscribe To Events
	m_eventSystem->Subscribe({ EventType::MouseMove }, std::bind(MouseMovedCallback, std::ref(m_eventSystem->m_data.mouse_x), std::ref(m_eventSystem->m_data.mouse_y)));
	m_eventSystem->Subscribe({ EventType::LeftMousePressed, EventType::RightMousePressed }, std::bind(MouseClickedCallback));
	m_eventSystem->Subscribe({ EventType::FocusLost }, std::bind(FocusLostCallback));
	m_eventSystem->Subscribe({ EventType::FocusGained }, std::bind(FocusGainedCallback));
	m_eventSystem->Subscribe({ EventType::KeyDown,  EventType::SysKeyDown }, std::bind(KeyboardCallback, std::ref(m_eventSystem->m_data.keycode)));
	m_eventSystem->Subscribe({ EventType::WindowResized }, std::bind(ResizeCallback, std::ref(m_eventSystem->m_data.screen_width), std::ref(m_eventSystem->m_data.screen_height)));
	ENGINE_INFO("Chilli Engine Initialized Successfully");
}

void ChilliEngine::Update()
{
	//Process Events
	m_eventSystem->ProcessFrame();
}

//Bridge Between Window And Engine
void ChilliEngine::RaiseEvent(EventData& data)
{
	Event* e = new Event(data);
	m_eventSystem->Push(e);
}

