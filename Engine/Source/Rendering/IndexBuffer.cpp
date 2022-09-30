#include "IndexBuffer.h"

namespace Chilli {

    IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices, const std::shared_ptr<Direct3D>& d3d)
        : m_count(static_cast<UINT>(indices.size())), m_direct3d(d3d)
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

        GFX_THROW_ERR(m_direct3d->GetDevice()->CreateBuffer(&index_buffer, &index_data, &m_indexBuffer));
    }

    void IndexBuffer::Bind() const
    {
        m_direct3d->GetContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
    }

    UINT IndexBuffer::GetCount() const
    {
        return m_count;
    }
}
