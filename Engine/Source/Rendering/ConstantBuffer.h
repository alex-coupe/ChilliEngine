#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <wrl.h>
#include "Renderer.h"
#include <vector>
#include <memory>

namespace Engine::Rendering {

	enum class ConstantBufferType {
		Vertex = 1, Pixel = 2
	};

	template <typename T>
	class CHILLI_API ConstantBuffer {
	public:
		//Creates Buffer With No Initial Data
		ConstantBuffer(const ConstantBufferType type,  std::shared_ptr<Renderer> renderer)
			:m_type(type), m_renderer(renderer)
		{
			D3D11_BUFFER_DESC constant_buffer = {};
			constant_buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constant_buffer.ByteWidth = sizeof(T);
			constant_buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constant_buffer.Usage = D3D11_USAGE_DYNAMIC;
			constant_buffer.MiscFlags = 0u;
			constant_buffer.StructureByteStride = 0u;

			if (FAILED(m_hresult = m_renderer->GetDevice()->CreateBuffer(&constant_buffer, nullptr, m_constantBuffer.GetAddressOf())))
			{
				m_renderer->GetDXError();
			}
		} 

		ConstantBuffer(const ConstantBufferType type, const T& data, std::shared_ptr<Renderer> renderer)
			: m_type(type), m_renderer(renderer)
		{
			D3D11_BUFFER_DESC constant_buffer = {};
			constant_buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constant_buffer.ByteWidth = sizeof(data);
			constant_buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constant_buffer.Usage = D3D11_USAGE_DYNAMIC;
			constant_buffer.MiscFlags = 0u;
			constant_buffer.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA constant_data = {};
			constant_data.pSysMem = &data;

			if (FAILED(m_hresult = m_renderer->GetDevice()->CreateBuffer(&constant_buffer, &constant_data, m_constantBuffer.GetAddressOf())))
			{
				m_renderer->GetDXError();
			}

		}

		void Bind(UINT slot = 0u)
		{
			switch (m_type)
			{
			case ConstantBufferType::Vertex:
				m_renderer->GetContext()->VSSetConstantBuffers(slot, 1u, m_constantBuffer.GetAddressOf());
				break;
			case ConstantBufferType::Pixel:
				m_renderer->GetContext()->PSSetConstantBuffers(slot, 1u, m_constantBuffer.GetAddressOf());
				break;
			}
		}

		void Update(const T& data)const
		{
			D3D11_SUBRESOURCE_DATA constant_data;

			if (FAILED(m_hresult = m_renderer->GetContext()->Map(m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &constant_data)))
			{
				m_renderer->GetDXError();
			}

			memcpy(constant_data.pSysMem, &data, sizeof(data));

			m_renderer->GetContext()->Unmap(m_constantBuffer.Get(), 0u);
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		std::shared_ptr<Renderer> m_renderer;
		ConstantBufferType m_type;
		HRESULT m_hresult = 0;
	};

}