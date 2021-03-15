#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Engine::Rendering {

	class CHILLI_API Sampler {
	public:
		Sampler(const std::shared_ptr<Direct3D>& d3d);
		void Bind(UINT slot = 0u)const;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
		std::shared_ptr<Direct3D> m_direct3d;
		HRESULT m_result = 0;
	};
}
