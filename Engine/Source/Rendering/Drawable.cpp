#include "Drawable.h"

namespace Chilli {

	Drawable::Drawable(const std::shared_ptr<Direct3D>& d3d, const std::shared_ptr<Entity>& entity)
		:m_direct3d(d3d), m_entity(entity)
	{
		auto transform = std::static_pointer_cast<TransformComponent>(m_entity->GetComponentByType(ComponentType::Transform));
		auto mesh = std::static_pointer_cast<MeshComponent>(m_entity->GetComponentByType(ComponentType::Mesh));
		
		if (transform != nullptr && mesh->GetMesh() != nullptr)
		{
			m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
			m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
			m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
			m_color->Bind();
		}

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		m_vertexShader = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Color.hlsl", m_direct3d);
		m_vertexShader->Bind();

		m_pixelShader = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Color.hlsl", m_direct3d);
		m_pixelShader->Bind();

		m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		m_inputLayout->Bind();

		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();
	}


	void Drawable::Draw() const
	{
		if (m_vertexBuffer && m_indexBuffer)
		{
			m_vertexBuffer->Bind();
			m_indexBuffer->Bind();
			m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
		}
	}

	const DirectX::XMMATRIX& Drawable::GetTransform()const
	{
		return m_transformMatrix;
	}

	void Drawable::Update()
	{
		const auto& mesh = std::static_pointer_cast<MeshComponent>(m_entity->GetComponentByType(ComponentType::Mesh));
		const auto& tranformComp = std::static_pointer_cast<TransformComponent>(m_entity->GetComponentByType(ComponentType::Transform));
		
		m_transformMatrix = tranformComp->GetTransformMatrix();
		if (mesh != nullptr)
		{
			m_color->Update(mesh->Color());
			if (m_indexBuffer == nullptr)
				m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);

			if (m_vertexBuffer == nullptr)
				m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);

			if (mesh->GetIndices().size() != m_indexBuffer->GetCount())
			{
				m_vertexBuffer.reset();
				m_indexBuffer.reset();
				m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
				m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
			}
		}
		else
		{
			m_vertexBuffer.reset();
			m_indexBuffer.reset();
		}
		
	}
}