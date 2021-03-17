#include "Topology.h"

Engine::Rendering::Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type, const std::shared_ptr<Direct3D>& d3d)
	:m_type(type), m_direct3d(d3d)
{
}

void Engine::Rendering::Topology::Bind()const
{
	m_direct3d->GetContext()->IASetPrimitiveTopology(m_type);
}
