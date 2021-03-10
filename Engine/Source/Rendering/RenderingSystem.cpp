#include "RenderingSystem.h"
#include <Windows.h>
#include "../Core/EventSystem.h"
#include "VertexTypes.h"

Engine::Rendering::RenderingSystem::RenderingSystem(HWND& handle, int64_t window_width, int64_t window_height, std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver)
	: EngineSystem(m_resolver)
{
	m_renderer = std::make_shared<Renderer>(handle, window_width, window_height);
}

int Engine::Rendering::RenderingSystem::GetHash()
{
    return static_cast<int>(Engine::Core::SystemTypes::Renderer);
	
}

void Engine::Rendering::RenderingSystem::Init()
{
	auto m_eventSystem = m_resolver->ResolveDependency<EventSystem>();
	m_eventSystem->Subscribe({ EventType::WindowResized },
		std::bind(&Renderer::HandleWindowResize, m_renderer, std::ref(m_eventSystem->m_data.screen_width), std::ref(m_eventSystem->m_data.screen_height)));
}

void Engine::Rendering::RenderingSystem::ProcessFrame()
{
	m_renderer->ProcessFrame();
}

void Engine::Rendering::RenderingSystem::CreateTestTriangle()
{
	VertexPos triangle;

	std::vector<VertexPos> vertices = {
		{{-1.0f,-1.0f,0.0f}},
		{{-1.0f,-1.0f,0.0f}},
		{{ 0.0f, 1.0f,0.0f}}
	};

	VertexBuffer vertexBuffer(vertices, m_renderer);
	
	
}
