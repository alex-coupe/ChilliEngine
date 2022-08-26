#include "Drawable.h"

Engine::Rendering::Drawable::Drawable(const std::shared_ptr<Direct3D>& d3d, const std::shared_ptr<Engine::ECS::Entity>& entity)
	:m_direct3d(d3d), m_entity(entity)
{
	auto transform = std::dynamic_pointer_cast<Engine::ECS::TransformComponent>(m_entity->GetComponentByType(Engine::ECS::ComponentTypes::Transform));
	auto mesh = std::dynamic_pointer_cast<Engine::ECS::MeshComponent>(m_entity->GetComponentByType(Engine::ECS::ComponentTypes::Mesh));
	m_transform = transform->GetTransformMatrix();
	m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
	
	m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
	
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

}

const DirectX::XMMATRIX& Engine::Rendering::Drawable::GetTransform() const
{
	return m_transform;
}

void Engine::Rendering::Drawable::Draw() const
{
	m_vertexBuffer->Bind();
	m_indexBuffer->Bind();
	m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
}

void Engine::Rendering::Drawable::Rebind()
{
}

void Engine::Rendering::Drawable::Update()
{
	m_transform =  std::dynamic_pointer_cast<Engine::ECS::TransformComponent>(m_entity->GetComponentByType(Engine::ECS::ComponentTypes::Transform))->GetTransformMatrix();
}
