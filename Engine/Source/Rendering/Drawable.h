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

#include "../ECS/Entity.h"
namespace Engine::Rendering {
	class CHILLI_API Drawable {
	public:
		Drawable(const std::shared_ptr<Direct3D>& d3d, const std::shared_ptr<Engine::ECS::Entity>& entity);
		const DirectX::XMMATRIX& GetTransform()const;
		void Draw()const;
		void Rebind();
		void Update();
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Shader> m_vertexShader;
		std::unique_ptr<Shader> m_pixelShader;
		std::unique_ptr<InputLayout> m_inputLayout;
		std::unique_ptr<Topology> m_topology;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		DirectX::XMMATRIX m_transform;
		const std::shared_ptr<Engine::ECS::Entity> m_entity;
	};
}