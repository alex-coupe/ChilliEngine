#include "VertexBuffer.h"

void Engine::Rendering::VertexBuffer::Bind()
{
	m_renderer->GetContext()->IASetVertexBuffers(0u, 1u, m_buffer.GetAddressOf(), &m_stride, 0u);
}
