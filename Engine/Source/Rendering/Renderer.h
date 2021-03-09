#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/EngineSystem.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

namespace Engine::Renderer  {

	using namespace Engine::Core;

#pragma comment (lib, "d3d11.lib") 
#pragma comment (lib, "D3DCompiler.lib")

	class CHILLI_API Renderer : public Engine::Core::EngineSystem {
	public:
		Renderer(HWND& handle, std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver);
		void BeginFrame();
		void EndFrame();
		int GetHash() override;
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
		HRESULT m_hresult;
	};

}