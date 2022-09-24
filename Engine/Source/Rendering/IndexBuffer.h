#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Chilli {

	class CHILLI_API IndexBuffer {
	public:
		IndexBuffer(const std::vector<unsigned short>& indices, const std::shared_ptr<Direct3D>& d3d);
		void Bind()const;
		UINT GetCount()const;
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
		UINT m_count;
		HRESULT m_result = 0;
	};

}