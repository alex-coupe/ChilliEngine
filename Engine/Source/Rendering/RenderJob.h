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
		void Draw(Light* light)const;
		void Update(Camera* cam, Light* light);
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Shader> m_vertexShaderTexture;
		std::unique_ptr<Shader> m_vertexShaderColor;
		std::unique_ptr<Shader> m_vertexShaderPhong;
		std::unique_ptr<Shader> m_pixelShaderTexture;
		std::unique_ptr<Shader> m_pixelShaderColor;
		std::unique_ptr<Shader> m_pixelShaderPhong;
		std::unique_ptr<Shader> m_pixelShaderPhongColor;
		std::unique_ptr<InputLayout> m_inputLayoutTexture;
		std::unique_ptr<InputLayout> m_inputLayoutColor;
		std::unique_ptr<InputLayout> m_inputLayoutPhong;
		std::unique_ptr<Topology> m_topology;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<Sampler> m_sampler;
		std::unique_ptr<Texture> m_texture;
		DirectX::XMMATRIX m_transformMatrix;
		std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4>> m_color;
		std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_transformationCBuff;
		Entity& m_entity;
		struct Transforms {
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProj;
		};
		struct PhongPixel {
			alignas(16)DirectX::XMFLOAT3 lightPos;
			alignas(16)DirectX::XMFLOAT3 lightCol;
			alignas(16)DirectX::XMFLOAT4 objCol;
			alignas(16)DirectX::XMFLOAT3 camPos;
			

		};
		std::unique_ptr<ConstantBuffer<Transforms>> m_phongConstantVertexBuffer;
		std::unique_ptr<ConstantBuffer<PhongPixel>> m_phongConstantPixelBuffer;
	};
}