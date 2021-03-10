#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <memory>
#include "Renderer.h"

namespace Engine::Rendering {

	class CHILLI_API Topology {
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY type, std::shared_ptr<Renderer> renderer);
		void Bind() const;
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_type;
		std::shared_ptr<Renderer> m_renderer;
	};

}