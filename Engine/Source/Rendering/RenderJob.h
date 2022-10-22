#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Sampler.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "../ECS/Entity.h"
#include "ShaderLibrary.h"

namespace Chilli {
	class CHILLI_API RenderJob {
	public:
		RenderJob(const std::shared_ptr<Direct3D>& d3d, Entity& entity);
		void Draw(Light* light)const;
		void Update(Camera* cam, Light* light);
		bool RenderDuringPlay();
	private:
		bool m_renderDuringPlay = true;
		void CreateSolidJob();
		void CreateMeshComponentJob(Light* light, std::shared_ptr<MeshComponent> mesh);
		std::shared_ptr<Direct3D> m_direct3d;
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<Shader> m_vertexShader;
		std::shared_ptr<Shader> m_pixelShader;
		std::unique_ptr<InputLayout> m_inputLayout;
		std::unique_ptr<Topology> m_topology;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<Sampler> m_sampler;
		DirectX::XMMATRIX m_transformMatrix;
		std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4>> m_color;
		std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_transformationCBuff;
		Entity& m_entity;
		struct Transforms {
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProj;
			alignas(16)DirectX::XMFLOAT3 camPos;
		};
		
		std::shared_ptr<Mesh> m_baseMesh;
		std::unique_ptr<ConstantBuffer<Transforms>> m_phongConstantVertexBuffer;
		std::unique_ptr<ConstantBuffer<LightProperties>> m_lightPropsConstantBuffer;
		std::unique_ptr<ConstantBuffer<Material>> m_materialConstantBuffer;
	};
}