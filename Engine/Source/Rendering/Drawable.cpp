#include "Drawable.h"

Engine::Rendering::Drawable::Drawable(const std::shared_ptr<Direct3D>& d3d, const std::vector<VertexPos>& vertices, const std::vector<unsigned short>& indices,
	DirectX::XMMATRIX model, DirectX::XMMATRIX proj)
	:m_direct3d(d3d)
{
	m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, m_direct3d);
	m_vertexBuffer->Bind();
	m_indexBuffer = std::make_unique<IndexBuffer>(indices, m_direct3d);
	m_indexBuffer->Bind();
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_vertexShader = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Basic.hlsl", m_direct3d);
	m_vertexShader->Bind();

	m_pixelShader = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Basic.hlsl", m_direct3d);
	m_pixelShader->Bind();

	m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
	m_inputLayout->Bind();

	m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
	m_topology->Bind();

	m_transform =  DirectX::XMMatrixTranspose(
		model * proj
	);

	m_modelProjMatrix = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_transform, m_direct3d);
	m_modelProjMatrix->Bind();

}

void Engine::Rendering::Drawable::Draw() const
{
	m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
}

void Engine::Rendering::Drawable::Rebind()
{
}
