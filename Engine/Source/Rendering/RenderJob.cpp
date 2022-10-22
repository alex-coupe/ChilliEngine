#include "RenderJob.h"

namespace Chilli {

	RenderJob::RenderJob(const std::shared_ptr<Direct3D>& d3d, Entity& entity)
		:m_direct3d(d3d), m_entity(entity)
	{
		if (m_entity.HasComponent(ComponentType::Camera))
		{
			m_renderDuringPlay = false;
			m_baseMesh = std::make_shared<Mesh>("Assets/Models/cam.fbx");
		}

		if (m_entity.HasComponent(ComponentType::Light))
		{
			m_renderDuringPlay = false;
			m_baseMesh = std::make_shared<Mesh>("Assets/Models/lamp.fbx");
		}

		auto transform = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));
		
		if (m_entity.HasComponent(ComponentType::Light) || m_entity.HasComponent(ComponentType::Camera))
		{
			CreateSolidJob();
			m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		}
		else
		{
			auto mesh = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));

			if (mesh && mesh->GetMesh() != nullptr)
			{
				m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
				m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
				m_phongConstantVertexBuffer = std::make_unique<ConstantBuffer<Transforms>>(ConstantBufferType::Vertex, m_direct3d);
				m_lightPropsConstantBuffer = std::make_unique<ConstantBuffer<LightProperties>>(ConstantBufferType::Pixel, m_direct3d);
				m_materialConstantBuffer = std::make_unique<ConstantBuffer<Material>>(ConstantBufferType::Pixel, m_direct3d);
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexPhong");
				if (mesh->HasTexture())
				{
					m_sampler = std::make_unique<Sampler>(m_direct3d);
					m_sampler->Bind();
					m_pixelShader = ShaderLibrary::GetCoreShader("PixelPhongTex");
				}
				else
				{
					m_pixelShader = ShaderLibrary::GetCoreShader("PixelPhongCol");
				}
				m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);

			}
		}
		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();
	}

	bool RenderJob::RenderDuringPlay()
	{
		return m_renderDuringPlay;
	}

	void RenderJob::Draw(Light* light) const
	{
		if (m_vertexBuffer && m_indexBuffer)
		{
			m_vertexBuffer->Bind();
			m_indexBuffer->Bind();
			m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
		}
	}

	void RenderJob::Update(Camera* cam, Light* light)
	{
		auto meshComponent = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));
		auto tranformComp = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));
			
		if (meshComponent && meshComponent->HasMesh())
		{
			CreateMeshComponentJob(light, meshComponent);
			
			if (light)
			{
				m_lightPropsConstantBuffer->Bind();
				m_phongConstantVertexBuffer->Bind();
				m_materialConstantBuffer->Bind(1);
				Transforms trans = {
					trans.model = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix()),
					trans.modelViewProj = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix()),
					trans.camPos = DirectX::XMFLOAT3(DirectX::XMVectorGetX(cam->GetPosition()),DirectX::XMVectorGetY(cam->GetPosition()),DirectX::XMVectorGetZ(cam->GetPosition()))
				};
							
				m_phongConstantVertexBuffer->Update(trans);
				light->Update();
				m_lightPropsConstantBuffer->Update(light->properties);
				m_materialConstantBuffer->Update(meshComponent->material);
			}
			else
			{
				auto transform = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
				m_transformationCBuff->Update(transform);
				m_transformationCBuff->Bind();

				m_color->Update(DirectX::XMFLOAT4(meshComponent->material.diffuse.x, meshComponent->material.diffuse.y, meshComponent->material.diffuse.z, 1.0f));
				m_color->Bind();
			}

			if (meshComponent->GetIndices().size() != m_indexBuffer->GetCount())
			{
				m_vertexBuffer.reset();
				m_indexBuffer.reset();
				m_vertexBuffer = std::make_unique<VertexBuffer>(meshComponent->GetVertices(), m_direct3d);
				m_indexBuffer = std::make_unique<IndexBuffer>(meshComponent->GetIndices(), m_direct3d);
			}

		}
		else if (m_entity.HasComponent(ComponentType::Light) || m_entity.HasComponent(ComponentType::Camera))
		{
			m_renderDuringPlay = false;
			if (!m_transformationCBuff)
				CreateSolidJob();

			auto transform = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
			m_transformationCBuff->Update(transform);
			m_transformationCBuff->Bind();

			if (m_entity.HasComponent(ComponentType::Light))
			{
				auto lightComp = std::static_pointer_cast<LightComponent>(m_entity.GetComponentByType(ComponentType::Light));
				m_color->Update(DirectX::XMFLOAT4(lightComp->Diffuse().x, lightComp->Diffuse().y, lightComp->Diffuse().z, 1.0f));
				m_color->Bind();
			}

		}
		else
		{
			m_vertexBuffer.reset();
			m_indexBuffer.reset();
		}

		if (m_inputLayout)
			m_inputLayout->Bind();
		if (m_vertexShader)
			m_vertexShader->Bind();
		if (m_pixelShader)
			m_pixelShader->Bind();
	}

	void RenderJob::CreateMeshComponentJob(Light* light, std::shared_ptr<MeshComponent> mesh)
	{
		m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
		m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
		if (light)
		{
			m_phongConstantVertexBuffer = std::make_unique<ConstantBuffer<Transforms>>(ConstantBufferType::Vertex, m_direct3d);
			m_lightPropsConstantBuffer = std::make_unique<ConstantBuffer<LightProperties>>(ConstantBufferType::Pixel, m_direct3d);
			m_materialConstantBuffer = std::make_unique<ConstantBuffer<Material>>(ConstantBufferType::Pixel, m_direct3d);
			m_vertexShader = ShaderLibrary::GetCoreShader("VertexPhong");
			if (mesh->HasTexture())
			{
				m_sampler = std::make_unique<Sampler>(m_direct3d);
				m_sampler->Bind();
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelPhongTex");
				mesh->GetTexture()->Bind();
			}
			else
			{
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelPhongCol");
			}
			m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);

		}
		else
		{
			m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
			m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
			if (mesh->HasTexture())
			{
				m_sampler = std::make_unique<Sampler>(m_direct3d);
				m_sampler->Bind();
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexTex");
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelTex");
				mesh->GetTexture()->Bind();
			}
			else
			{
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexCol");
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelCol");
			}
			m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		}
	}

	void RenderJob::CreateSolidJob()
	{
		m_vertexBuffer = std::make_unique<VertexBuffer>(m_baseMesh->GetVertices(), m_direct3d);
		m_indexBuffer = std::make_unique<IndexBuffer>(m_baseMesh->GetIndices(), m_direct3d);
		if (m_entity.HasComponent(ComponentType::Light))
		{
			m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
			m_pixelShader = ShaderLibrary::GetCoreShader("PixelCol");
		}
		else
			m_pixelShader = ShaderLibrary::GetCoreShader("PixelBasic");

		m_vertexShader = ShaderLibrary::GetCoreShader("VertexCol");
		
		m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
	}
}