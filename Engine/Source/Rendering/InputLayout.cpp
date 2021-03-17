#include "InputLayout.h"

Engine::Rendering::InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexByteCode, const std::shared_ptr<Direct3D>& d3d)
	: m_direct3d(d3d)
{
	GFX_THROW_ERR(m_direct3d->GetDevice()->CreateInputLayout(layout.data(), (UINT)layout.size(), vertexByteCode->GetBufferPointer(),
		vertexByteCode->GetBufferSize(), &m_inputLayout));
	
}

void Engine::Rendering::InputLayout::Bind()const
{
	m_direct3d->GetContext()->IASetInputLayout(m_inputLayout.Get());
}
