#include "Renderer.h"

Engine::Renderer::Renderer::Renderer(HWND& handle, int64_t window_width, int64_t window_height, std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver)
	:  EngineSystem(m_resolver)
{
}

int Engine::Renderer::Renderer::GetHash()
{
	return static_cast<int>(SystemTypes::Renderer);
}
