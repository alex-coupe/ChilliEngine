#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <memory>
#include "Direct3D.h"

namespace Engine::Rendering {

	class CHILLI_API Topology {
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY type, const std::shared_ptr<Direct3D>& d3d);
		void Bind() const;
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_type;
		std::shared_ptr<Direct3D> m_direct3d;
	};

}