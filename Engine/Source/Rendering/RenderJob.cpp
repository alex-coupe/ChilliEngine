#include "RenderJob.h"

namespace Chilli {

	RenderJob::RenderJob(const std::shared_ptr<Direct3D>& d3d, Entity& entity)
		:m_direct3d(d3d), m_entity(entity)
	{
		auto transform = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));
		auto mesh = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));
		
		if (transform != nullptr && mesh->GetMesh() != nullptr)
		{
			m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
			m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
			m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
			m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
		}

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		m_vertexShader = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Texture.hlsl", m_direct3d);
		m_vertexShader->Bind();

		m_pixelShader = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Texture.hlsl", m_direct3d);
		m_pixelShader->Bind();

		m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		m_inputLayout->Bind();

		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();

		m_sampler = std::make_unique<Sampler>(m_direct3d);
		m_sampler->Bind();

		m_texture = std::make_unique<Texture>("Assets/Materials/container2.png", m_direct3d);
		m_texture->Bind();
	}


	void RenderJob::Draw() const
	{
		if (m_vertexBuffer && m_indexBuffer)
		{
			m_transformationCBuff->Bind();
			m_color->Bind();
			m_vertexBuffer->Bind();
			m_indexBuffer->Bind();
			m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
		}
	}

	void RenderJob::Update(Camera* cam)
	{
		const auto& meshComponent = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));
		const auto& tranformComp = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));
		
		if (meshComponent->HasMesh())
		{
			if (!m_transformationCBuff)
				m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
			
			if (!m_color)
				m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);

			auto transform = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
			m_transformationCBuff->Update(transform);

			m_color->Update(meshComponent->Color());
			if (m_indexBuffer == nullptr)
				m_indexBuffer = std::make_unique<IndexBuffer>(meshComponent->GetIndices(), m_direct3d);

			if (m_vertexBuffer == nullptr)
				m_vertexBuffer = std::make_unique<VertexBuffer>(meshComponent->GetVertices(), m_direct3d);

			if (meshComponent->GetIndices().size() != m_indexBuffer->GetCount())
			{
				m_vertexBuffer.reset();
				m_indexBuffer.reset();
				m_vertexBuffer = std::make_unique<VertexBuffer>(meshComponent->GetVertices(), m_direct3d);
				m_indexBuffer = std::make_unique<IndexBuffer>(meshComponent->GetIndices(), m_direct3d);
			}
		}
		else
		{
			m_vertexBuffer.reset();
			m_indexBuffer.reset();
		}
		
	}
}