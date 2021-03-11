#include "Topology.h"

Engine::Rendering::Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type, std::shared_ptr<Renderer> renderer)
	:m_type(type), m_renderer(renderer)
{
}

void Engine::Rendering::Topology::Bind()const
{
	m_renderer->GetContext()->IASetPrimitiveTopology(m_type);
}
