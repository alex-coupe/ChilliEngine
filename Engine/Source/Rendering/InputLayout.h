#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <memory>
#include <vector>
#include "Renderer.h"

namespace Engine::Rendering {

	class CHILLI_API InputLayout {
	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexByteCode, std::shared_ptr<Renderer> renderer);
		void Bind();
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		HRESULT m_hresult = 0;
		std::shared_ptr<Renderer> m_renderer;
	};

}
