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

		m_vertexShaderTexture = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Texture.hlsl", m_direct3d);
		m_vertexShaderColor = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Color.hlsl", m_direct3d);
		m_vertexShaderPhong = std::make_unique<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Phong.hlsl", m_direct3d);
		
		m_pixelShaderTexture = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Texture.hlsl", m_direct3d);
		m_pixelShaderColor = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Color.hlsl", m_direct3d);
		m_pixelShaderPhong = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Phong.hlsl", m_direct3d);
		m_pixelShaderPhongColor = std::make_unique<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_PhongCol.hlsl", m_direct3d);
		
		m_inputLayoutTexture = std::make_unique<InputLayout>(ied, m_vertexShaderTexture->GetByteCode(), m_direct3d);
		m_inputLayoutColor = std::make_unique<InputLayout>(ied, m_vertexShaderColor->GetByteCode(), m_direct3d);
		m_inputLayoutPhong = std::make_unique<InputLayout>(ied, m_vertexShaderPhong->GetByteCode(), m_direct3d);
		
		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();

		m_sampler = std::make_unique<Sampler>(m_direct3d);
		m_sampler->Bind();
	}


	void RenderJob::Draw(Light* light) const
	{
		if (m_vertexBuffer && m_indexBuffer)
		{
			if (m_entity.HasComponent(ComponentType::Light) || !light)
			{
				m_transformationCBuff->Bind();
				m_color->Bind();
			}
			else
			{
				m_phongConstantVertexBuffer->Bind();
				m_phongConstantPixelBuffer->Bind();
			}
			m_vertexBuffer->Bind();
			m_indexBuffer->Bind();
			m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
		}
	}

	void RenderJob::Update(Camera* cam, Light* light)
	{
		const auto& meshComponent = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));
		const auto& tranformComp = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));
	
		if (meshComponent->HasMesh())
		{
			if (!m_entity.HasComponent(ComponentType::Light) && light)
			{
				m_inputLayoutPhong->Bind();
				m_vertexShaderPhong->Bind();

				if (meshComponent->HasTexture())
				{
					meshComponent->GetTexture()->Bind();
					m_pixelShaderPhong->Bind();
				}
				else
				{
					m_pixelShaderPhongColor->Bind();
				}
				
				if (!m_phongConstantVertexBuffer)
					m_phongConstantVertexBuffer = std::make_unique<ConstantBuffer<Transforms>>(ConstantBufferType::Vertex, m_direct3d);
				if (!m_phongConstantPixelBuffer)
					m_phongConstantPixelBuffer = std::make_unique<ConstantBuffer<PhongPixel>>(ConstantBufferType::Pixel, m_direct3d);

				Transforms trans = {
					trans.model = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix()),
					trans.modelViewProj = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix())
				};

				PhongPixel pp = {
					pp.lightPos = light->GetPosition(),
					pp.lightCol = light->GetColor(),
					pp.objCol = meshComponent->material.color,
					pp.camPos = DirectX::XMFLOAT3{DirectX::XMVectorGetX(cam->GetPosition()),DirectX::XMVectorGetY(cam->GetPosition()),DirectX::XMVectorGetZ(cam->GetPosition())}
				};

				m_phongConstantVertexBuffer->Update(trans);
				m_phongConstantPixelBuffer->Update(pp);

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
				
				return;
			}

			if (meshComponent->HasTexture() && light == nullptr)
			{
				m_inputLayoutTexture->Bind();
				m_vertexShaderTexture->Bind();
				m_pixelShaderTexture->Bind();
				meshComponent->GetTexture()->Bind();
			}
			else
			{
				m_inputLayoutColor->Bind();
				m_vertexShaderColor->Bind();
				m_pixelShaderColor->Bind();
			}

			if (!m_transformationCBuff)
				m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
			
			if (!m_color)
				m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);

			auto transform = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
			m_transformationCBuff->Update(transform);

			m_color->Update(meshComponent->material.color);
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