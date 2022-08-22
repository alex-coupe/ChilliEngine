#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Engine::Rendering {

	enum class ConstantBufferType {
		Vertex = 1, Pixel = 2
	};

	template <typename T>
	class CHILLI_API ConstantBuffer {
	public:
		//Creates Buffer With No Initial Data
		ConstantBuffer(const ConstantBufferType type,  const std::shared_ptr<Direct3D>& d3d)
			:m_type(type), m_direct3d(d3d)
		{
			D3D11_BUFFER_DESC constant_buffer = {};
			constant_buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constant_buffer.ByteWidth = sizeof(T);
			constant_buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constant_buffer.Usage = D3D11_USAGE_DYNAMIC;
			constant_buffer.MiscFlags = 0u;
			constant_buffer.StructureByteStride = 0u;

			GFX_THROW_ERR(m_direct3d->GetDevice()->CreateBuffer(&constant_buffer, nullptr, m_constantBuffer.GetAddressOf()));
			
		} 

		ConstantBuffer(const ConstantBufferType type, const T& data, const std::shared_ptr<Direct3D>& d3d)
			: m_type(type), m_direct3d(d3d)
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

			GFX_THROW_ERR(m_direct3d->GetDevice()->CreateBuffer(&constant_buffer, &constant_data, m_constantBuffer.GetAddressOf()));
			
		}

		void Bind(UINT slot = 0u)
		{
			switch (m_type)
			{
			case ConstantBufferType::Vertex:
				m_direct3d->GetContext()->VSSetConstantBuffers(slot, 1u, m_constantBuffer.GetAddressOf());
				break;
			case ConstantBufferType::Pixel:
				m_direct3d->GetContext()->PSSetConstantBuffers(slot, 1u, m_constantBuffer.GetAddressOf());
				break;
			}
		}

		void Update(const T& data)
		{
			D3D11_MAPPED_SUBRESOURCE constant_data;

			GFX_THROW_ERR(m_direct3d->GetContext()->Map(m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &constant_data));
			
			memcpy(constant_data.pData, &data, sizeof(data));

			m_direct3d->GetContext()->Unmap(m_constantBuffer.Get(), 0u);
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		std::shared_ptr<Direct3D> m_direct3d;
		ConstantBufferType m_type;
		HRESULT m_result = 0;
	};

}