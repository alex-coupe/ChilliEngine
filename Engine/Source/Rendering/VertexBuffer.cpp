#include "VertexBuffer.h"

void Engine::Rendering::VertexBuffer::Bind()
{
	UINT m_offset = 0u;
	m_direct3d->GetContext()->IASetVertexBuffers(0u, 1u, m_buffer.GetAddressOf(), &m_stride, &m_offset);
}
