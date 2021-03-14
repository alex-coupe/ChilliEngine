#include "Cube.h"
#include "../VertexTypes.h"

Engine::Rendering::Cube::Cube(const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d)
{
	
	std::vector<VertexPosTex> vertices = {
		{{ -1.0f,-1.0f,-1.0f}, {0.0f,1.0f}},
		{{ 1.0f,-1.0f,-1.0f }, {1.0f,1.0f}},
		{{ -1.0f,1.0f,-1.0f }, {0.0f,0.0f}},
		{{ 1.0f,1.0f, -1.0f }, {1.0f,0.0f}},

		{{ -1.0f,-1.0f,1.0f }, {0.0f,1.0f}},
		{{ 1.0f,-1.0f, 1.0f }, {1.0f,1.0f}},
		{{ -1.0f,1.0f, 1.0f }, {0.0f,0.0f}},
		{{ 1.0f,1.0f,  1.0f }, {1.0f,0.0f}},

		{{ 1.0f,-1.0f,-1.0f }, {0.0f,1.0f}}, //bottom right front
		{{ 1.0f,-1.0f, 1.0f }, {1.0f,1.0f}}, //bottom right back
		{{ 1.0f,1.0f, -1.0f }, {0.0f,0.0f}}, //top right front
		{{ 1.0f,1.0f,  1.0f }, {1.0f,0.0f}}, //top right back

		{{ -1.0f,-1.0f,1.0f }, {0.0f,1.0f}}, //back bottom left
		{{ -1.0f,-1.0f,-1.0f}, {1.0f,1.0f}}, //front bottom left
		{{ -1.0f,1.0f, 1.0f }, {0.0f,0.0f}}, //back top left
		{{ -1.0f,1.0f,-1.0f }, {1.0f,0.0f}}, //front top left

		{{ -1.0f,-1.0f,1.0f }, {0.0f,1.0f}},
		{{ 1.0f,-1.0f, 1.0f }, {1.0f,1.0f}},
		{{ -1.0f,-1.0f,-1.0f}, {0.0f,0.0f}},
		{{ 1.0f,-1.0f,-1.0f }, {1.0f,0.0f}},

		{{ -1.0f,1.0f,-1.0f}, {0.0f,1.0f}},
		{{ 1.0f,1.0f, -1.0f }, {1.0f,1.0f}},
		{{ -1.0f,1.0f, 1.0f }, {0.0f,0.0f}},
		{{ 1.0f,1.0f,  1.0f }, {1.0f,0.0f}},
	};

	const std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		4,6,5, 6,7,5,
		8,10,9, 10,11,9,
		12,14,13, 14,15,13,

		16,18,17, 18,19,17,
		20,22,21, 22,23,21
	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, m_direct3d);
	m_vertexBuffer->Bind();

	m_vertexShader = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Texture.hlsl", m_direct3d);
	m_vertexShader->Bind();

	m_pixelShader = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Texture.hlsl", m_direct3d);
	m_pixelShader->Bind();

	m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
	m_inputLayout->Bind();

	m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
	m_topology->Bind();

	float aspectRatio = static_cast<float>(m_direct3d->GetWindowHeight()) / static_cast<float>(m_direct3d->GetWindowWidth());

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.0f) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) *
		DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio, 0.5f, 100.0f)
	);

	m_constantBuffer = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, transform, m_direct3d);
	m_constantBuffer->Bind();


	m_indexBuffer = std::make_unique<IndexBuffer>(indices, m_direct3d);
	m_indexBuffer->Bind();

	m_texture = std::make_unique<Texture>("Assets/Materials/container2.png", m_direct3d);
	m_texture->Bind();

	m_sampler = std::make_unique<Sampler>(m_direct3d);
	m_sampler->Bind();

}

void Engine::Rendering::Cube::Draw() const
{
	m_direct3d->DrawIndexed(36);
}

void Engine::Rendering::Cube::Rebind()
{
	

}
