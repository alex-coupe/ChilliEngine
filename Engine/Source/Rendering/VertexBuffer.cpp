#include "VertexBuffer.h"

void Engine::Rendering::VertexBuffer::Bind()
{
	UINT m_offset = 0u;
	m_renderer->GetContext()->IASetVertexBuffers(0u, 1u, m_buffer.GetAddressOf(), &m_stride, &m_offset);
}
