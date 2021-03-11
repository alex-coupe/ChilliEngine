#include "IndexBuffer.h"

Engine::Rendering::IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices, std::shared_ptr<Renderer> renderer)
    : m_count(static_cast<UINT>(indices.size())), m_renderer(renderer)
{
    D3D11_BUFFER_DESC index_buffer = {};

    index_buffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buffer.Usage = D3D11_USAGE_DEFAULT;
    index_buffer.CPUAccessFlags = 0;
    index_buffer.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * indices.size());
    index_buffer.MiscFlags = 0;
    index_buffer.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA index_data = {};
    index_data.pSysMem = indices.data();

    if (FAILED(m_hresult = m_renderer->GetDevice()->CreateBuffer(&index_buffer, &index_data, &m_indexBuffer)))
    {
        m_renderer->GetDXError();
    }

}

void Engine::Rendering::IndexBuffer::Bind() const
{
    m_renderer->GetContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT Engine::Rendering::IndexBuffer::GetCount() const
{
    return m_count;
}
