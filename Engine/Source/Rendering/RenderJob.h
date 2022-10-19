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

namespace Chilli {
	class CHILLI_API RenderJob {
	public:
		RenderJob(const std::shared_ptr<Direct3D>& d3d, Entity& entity);
		void Draw()const;
		void Update(Camera* cam);
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Shader> m_vertexShader;
		std::unique_ptr<Shader> m_pixelShader;
		std::unique_ptr<InputLayout> m_inputLayout;
		std::unique_ptr<Topology> m_topology;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		DirectX::XMMATRIX m_transformMatrix;
		std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4>> m_color;
		std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_transformationCBuff;
		Entity& m_entity;
	};
}