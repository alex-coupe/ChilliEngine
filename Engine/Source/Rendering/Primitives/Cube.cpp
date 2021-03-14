#include "Cube.h"
#include "../VertexTypes.h"

Engine::Rendering::Cube::Cube(const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d)
{
	m_texture = std::make_unique<Texture>("C:\\Users\\Alexander\\Documents\\Repos\\ChilliEngine\\Application\\Assets\\Materials\\wall.jpg");
	std::vector<VertexPos> vertices = {
		{{ -1.0f,-1.0f,-1.0f }},
		{{ 1.0f,-1.0f,-1.0f }},
		{{ -1.0f,1.0f,-1.0f }},
		{{ 1.0f,1.0f, -1.0f }},
		{{ -1.0f,-1.0f,1.0f }},
		{{ 1.0f,-1.0f,1.0f }},
		{{ -1.0f,1.0f,1.0f }},
		{{ 1.0f,1.0f,1.0f }},

	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, m_direct3d);
	m_vertexBuffer->Bind();

	m_vertexShader = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Basic.hlsl", m_direct3d);
	m_vertexShader->Bind();

	m_pixelShader = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Basic.hlsl", m_direct3d);
	m_pixelShader->Bind();

	m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
	m_inputLayout->Bind();

	m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
	m_topology->Bind();

	std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4

	};

	m_indexBuffer = std::make_unique<IndexBuffer>(indices, m_direct3d);
	m_indexBuffer->Bind();

	float aspectRatio = static_cast<float>(m_direct3d->GetWindowHeight()) / static_cast<float>(m_direct3d->GetWindowWidth());

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.0f) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) *
		DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio, 0.5f, 100.0f)
	);

	m_constantBuffer = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, transform, m_direct3d);
	m_constantBuffer->Bind();
}

void Engine::Rendering::Cube::Draw() const
{
	m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
}

void Engine::Rendering::Cube::Rebind()
{
	m_constantBuffer.reset();
	m_indexBuffer.reset();
	m_inputLayout.reset();
	m_pixelShader.reset();
	m_topology.reset();
	m_vertexBuffer.reset();
	m_vertexShader.reset();

	std::vector<VertexPos> vertices = {
		{{ -1.0f,-1.0f,-1.0f }},
		{{ 1.0f,-1.0f,-1.0f }},
		{{ -1.0f,1.0f,-1.0f }},
		{{ 1.0f,1.0f, -1.0f }},
		{{ -1.0f,-1.0f,1.0f }},
		{{ 1.0f,-1.0f,1.0f }},
		{{ -1.0f,1.0f,1.0f }},
		{{ 1.0f,1.0f,1.0f }},

	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, m_direct3d);
	m_vertexBuffer->Bind();

	m_vertexShader = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Basic.hlsl", m_direct3d);
	m_vertexShader->Bind();

	m_pixelShader = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Basic.hlsl", m_direct3d);
	m_pixelShader->Bind();

	m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
	m_inputLayout->Bind();

	m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
	m_topology->Bind();

	std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4

	};

	m_indexBuffer = std::make_unique<IndexBuffer>(indices, m_direct3d);
	m_indexBuffer->Bind();

	float aspectRatio = static_cast<float>(m_direct3d->GetWindowHeight()) / static_cast<float>(m_direct3d->GetWindowWidth());

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.0f) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) *
		DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio, 0.5f, 100.0f)
	);

	m_constantBuffer = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, transform, m_direct3d);
	m_constantBuffer->Bind();
}
