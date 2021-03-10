#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/EngineSystem.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace Engine::Renderer  {

	using namespace Engine::Core;

#pragma comment (lib, "d3d11.lib") 
#pragma comment (lib, "D3DCompiler.lib")

	class CHILLI_API Renderer : public Engine::Core::EngineSystem {
	public:
		Renderer(HWND& handle, int64_t window_width, int64_t window_height, std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver);
		void ProcessFrame();
		int GetHash() override;
		void HandleWindowResize(int64_t width, int64_t height);
	private:
		void BeginFrame();
		void EndFrame();
		
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBuffer;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_bufferTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D11Debug> m_debug;
		HRESULT m_hresult = 0;
		HWND& m_handle;
		int64_t m_width;
		int64_t m_height;
	};

}