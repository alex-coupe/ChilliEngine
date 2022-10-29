#include "RenderJob.h"

namespace Chilli {

	RenderJob::RenderJob(const std::shared_ptr<Direct3D>& d3d, Entity& entity, RenderJobType type)
		:m_direct3d(d3d), m_entity(entity), m_renderJobType(type)
	{
		m_lightCountBuffer = std::make_unique<ConstantBuffer<LightCount>>(ConstantBufferType::Pixel, d3d);
		LightCount count = {};
		count.dirLightCount = 1;
		m_lightCountBuffer->Update(count);
		m_lightCountBuffer->Bind(4);
		switch (m_renderJobType)
		{
		case RenderJobType::Camera:
			CreateCameraJob();
			break;
		case RenderJobType::Light:
			CreateLightCasterJob();
			break;
		case RenderJobType::Mesh:
			CreateMeshJob();
			break;
		}
	}

	void RenderJob::CreateCameraJob()
	{
		Mesh mesh("Assets/Models/cam.fbx");
		m_vertexBuffer = std::make_unique<VertexBuffer>(mesh.GetVertices(), m_direct3d);
		m_indexBuffer = std::make_unique<IndexBuffer>(mesh.GetIndices(), m_direct3d);

		m_pixelShader = ShaderLibrary::GetCoreShader("PixelBasic");
		m_vertexShader = ShaderLibrary::GetCoreShader("VertexCol");
		m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);

		m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();
	}

	void RenderJob::UpdateCameraJob(Camera* cam)
	{
		auto tranformComp = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));
		auto transform = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
		m_transformationCBuff->Update(transform);
		m_transformationCBuff->Bind();
		m_inputLayout->Bind();
		m_vertexShader->Bind();
		m_pixelShader->Bind();
	}

	void RenderJob::CreateLightCasterJob()
	{
		Mesh mesh("Assets/Models/lamp.fbx");
		m_vertexBuffer = std::make_unique<VertexBuffer>(mesh.GetVertices(), m_direct3d);
		m_indexBuffer = std::make_unique<IndexBuffer>(mesh.GetIndices(), m_direct3d);

		m_pixelShader = ShaderLibrary::GetCoreShader("PixelCol");
		m_vertexShader = ShaderLibrary::GetCoreShader("VertexCol");

		m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
		m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
		
		m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();
	}

	void RenderJob::UpdateLightCasterJob(Camera* cam)
	{
		auto transformComp = m_entity.GetTransformComponent();
		auto transform = DirectX::XMMatrixTranspose(transformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
		m_transformationCBuff->Update(transform);
		m_transformationCBuff->Bind();		
		auto lightComp = std::static_pointer_cast<LightComponent>(m_entity.GetComponentByType(ComponentType::Light));
		m_color->Update(DirectX::XMFLOAT4(lightComp->Diffuse().x, lightComp->Diffuse().y, lightComp->Diffuse().z, 1.0f));
		m_color->Bind();
		m_inputLayout->Bind();
		m_vertexShader->Bind();		
		m_pixelShader->Bind();
	}

	void RenderJob::CreateMeshJob()
	{
		auto mesh = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));

		if (mesh && mesh->GetMesh() != nullptr)
		{
			m_vertexBuffer = std::make_unique<VertexBuffer>(mesh->GetVertices(), m_direct3d);
			m_indexBuffer = std::make_unique<IndexBuffer>(mesh->GetIndices(), m_direct3d);
			m_phongConstantVertexBuffer = std::make_unique<ConstantBuffer<Transforms>>(ConstantBufferType::Vertex, m_direct3d);
			m_dirLightData = std::make_unique<ConstantBuffer<DirectionalLightData>>(ConstantBufferType::Pixel, m_direct3d);
			m_spotLightData = std::make_unique<ConstantBuffer<SpotlightData>>(ConstantBufferType::Pixel, m_direct3d);
			m_pointLightData = std::make_unique<ConstantBuffer<PointLightData>>(ConstantBufferType::Pixel, m_direct3d);
			m_materialConstantBuffer = std::make_unique<ConstantBuffer<Material>>(ConstantBufferType::Pixel, m_direct3d);
			
			if (mesh->HasTexture())
			{
				m_sampler = std::make_unique<Sampler>(m_direct3d);
				m_sampler->Bind();
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelTex");
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexTex");
			}
			else
			{
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelCol");
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexCol");
			}
			m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		}
		m_topology = std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
		m_topology->Bind();
	}

	void RenderJob::UpdateMeshJob(Camera* cam, SceneState state, Light* light)
	{
		auto meshComponent = std::static_pointer_cast<MeshComponent>(m_entity.GetComponentByType(ComponentType::Mesh));
		auto tranformComp = std::static_pointer_cast<TransformComponent>(m_entity.GetComponentByType(ComponentType::Transform));

		if (!meshComponent || !meshComponent->HasMesh())
			return;

		auto mesh = meshComponent->GetMesh();

		if (!m_vertexBuffer || !m_indexBuffer)
			CreateMeshJob();

		if (light && state == SceneState::Play)
		{
			if (!m_phongConstantVertexBuffer)
				m_phongConstantVertexBuffer = std::make_unique<ConstantBuffer<Transforms>>(ConstantBufferType::Vertex, m_direct3d);
						
			Transforms trans = {
				trans.model = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix()),
				trans.modelViewProj = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix()),
				trans.camPos = DirectX::XMFLOAT3(DirectX::XMVectorGetX(cam->GetPosition()),DirectX::XMVectorGetY(cam->GetPosition()),DirectX::XMVectorGetZ(cam->GetPosition()))
			};
			m_phongConstantVertexBuffer->Bind();
			m_phongConstantVertexBuffer->Update(trans);
			
			m_materialConstantBuffer->Update(meshComponent->material);
			m_materialConstantBuffer->Bind(0);
			light->Update();
			switch (light->GetLightType())
			{
			case LightType::DirectionalLight:
				if (!m_dirLightData)
					m_dirLightData = std::make_unique<ConstantBuffer<DirectionalLightData>>(ConstantBufferType::Pixel, m_direct3d);
				m_dirLightData->Update(static_cast<DirectionalLight*>(light)->data);
				m_dirLightData->Bind(1);
				break;
			case LightType::PointLight:
				if (!m_pointLightData)
					m_pointLightData = std::make_unique<ConstantBuffer<PointLightData>>(ConstantBufferType::Pixel, m_direct3d);
				m_pointLightData->Update(static_cast<PointLight*>(light)->data);
				m_pointLightData->Bind(2);
				break;
			case LightType::Spotlight:
				if (!m_spotLightData)
					m_spotLightData = std::make_unique<ConstantBuffer<SpotlightData>>(ConstantBufferType::Pixel, m_direct3d);
				m_spotLightData->Update(static_cast<Spotlight*>(light)->data);
				m_spotLightData->Bind(3);
				break;
			}
			m_materialConstantBuffer = std::make_unique<ConstantBuffer<Material>>(ConstantBufferType::Pixel, m_direct3d);
			m_vertexShader = ShaderLibrary::GetCoreShader("VertexPhong");
			if (meshComponent->HasTexture())
			{
				m_sampler = std::make_unique<Sampler>(m_direct3d);
				m_sampler->Bind();
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelPhongTex");
				meshComponent->GetTexture()->Bind();
				if (meshComponent->HasSpecularMap())
					meshComponent->GetSpecularMap()->Bind(1);
			}
			else
			{
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelPhongCol");
			}
			m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		}
		else
		{
			if (!m_transformationCBuff)
				m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
			if (!m_color)
				m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
			
			auto transform = DirectX::XMMatrixTranspose(tranformComp->GetTransformMatrix() * cam->GetViewProjMatrix());
			m_transformationCBuff->Update(transform);
			m_transformationCBuff->Bind();

			m_color->Update(DirectX::XMFLOAT4(meshComponent->material.diffuse.x, meshComponent->material.diffuse.y, meshComponent->material.diffuse.z, 1.0f));
			m_color->Bind();
			
			if (meshComponent->HasTexture())
			{
				if (!m_sampler)
					m_sampler = std::make_unique<Sampler>(m_direct3d);

				m_sampler->Bind();
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexTex");
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelTex");
				meshComponent->GetTexture()->Bind();
			}
			else
			{
				m_vertexShader = ShaderLibrary::GetCoreShader("VertexCol");
				m_pixelShader = ShaderLibrary::GetCoreShader("PixelCol");
			}
			if (!m_inputLayout)
				m_inputLayout = std::make_unique<InputLayout>(ied, m_vertexShader->GetByteCode(), m_direct3d);
		}

		if (meshComponent->GetIndices().size() != m_indexBuffer->GetCount())
		{
			m_vertexBuffer.reset();
			m_indexBuffer.reset();
			m_vertexBuffer = std::make_unique<VertexBuffer>(meshComponent->GetVertices(), m_direct3d);
			m_indexBuffer = std::make_unique<IndexBuffer>(meshComponent->GetIndices(), m_direct3d);
		}

		if (m_inputLayout)
			m_inputLayout->Bind();
		if (m_vertexShader)
			m_vertexShader->Bind();
		if (m_pixelShader)
			m_pixelShader->Bind();
	}

	void RenderJob::Draw(SceneState state) const
	{
		switch (m_renderJobType)
		{
		case RenderJobType::Camera:
		case RenderJobType::Light:
			if (state != SceneState::Play)
			{
				m_vertexBuffer->Bind();
				m_indexBuffer->Bind();
				m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
			}
			break;
		case RenderJobType::Mesh:
			if (m_vertexBuffer && m_indexBuffer)
			{
				m_vertexBuffer->Bind();
				m_indexBuffer->Bind();
				m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
			}
		}
		
	}

	void RenderJob::Update(Camera* cam, Light* light, SceneState currState)
	{
		switch (m_renderJobType)
		{
		case RenderJobType::Camera:
			UpdateCameraJob(cam);
			break;
		case RenderJobType::Light:
			UpdateLightCasterJob(cam);
			break;
		case RenderJobType::Mesh:
			UpdateMeshJob(cam, currState, light);
			break;
		}
	}
}