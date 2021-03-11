#include "InputLayout.h"

Engine::Rendering::InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexByteCode, std::shared_ptr<Renderer> renderer)
	: m_renderer(renderer)
{
	if (FAILED(m_hresult = m_renderer->GetDevice()->CreateInputLayout(layout.data(), (UINT)layout.size(), vertexByteCode->GetBufferPointer(),
		vertexByteCode->GetBufferSize(), &m_inputLayout)))
	{
		m_renderer->GetDXError();
	}
}

void Engine::Rendering::InputLayout::Bind()
{
	m_renderer->GetContext()->IASetInputLayout(m_inputLayout.Get());
}
