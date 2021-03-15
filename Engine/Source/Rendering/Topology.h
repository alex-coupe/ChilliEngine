#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Engine::Rendering {

	class CHILLI_API Topology {
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY type, const std::shared_ptr<Direct3D>& d3d);
		void Bind() const;
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_type;
		std::shared_ptr<Direct3D> m_direct3d;
		HRESULT m_result = 0;
	};

}