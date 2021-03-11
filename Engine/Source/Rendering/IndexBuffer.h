#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <wrl.h>
#include "Renderer.h"
#include <vector>
#include <memory>

namespace Engine::Rendering {

	class CHILLI_API IndexBuffer {
	public:
		IndexBuffer(const std::vector<unsigned short>& indices, std::shared_ptr<Renderer> renderer);
		void Bind()const;
		UINT GetCount()const;
	private:
		std::shared_ptr<Renderer> m_renderer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
		UINT m_count;
		HRESULT m_hresult =0;
	};

}