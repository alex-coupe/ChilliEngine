#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Engine::Rendering {

	class CHILLI_API InputLayout {
	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexByteCode, const std::shared_ptr<Direct3D>& d3d);
		void Bind()const;
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		std::shared_ptr<Direct3D> m_direct3d;
		HRESULT m_result = 0;
	};

}
