#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#pragma warning(disable:4251)
namespace Engine::Rendering {

#pragma comment (lib, "d3d11.lib") 
#pragma comment (lib, "D3DCompiler.lib")

	class CHILLI_API Renderer  {
	public:
		Renderer(HWND& handle, int64_t window_width, int64_t window_height);
		void ProcessFrame();
		void HandleWindowResize(int64_t width, int64_t height);
		Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	private:
		void BeginFrame();
		void EndFrame();
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBuffer;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_bufferTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D11InfoQueue> m_debugInfo;
		HRESULT m_hresult = 0;
		HWND& m_handle;
		int64_t m_width;
		int64_t m_height;
	};

}

#define GET_DXERROR UINT64 message_count = m_debugInfo->GetNumStoredMessages();\
for (UINT64 i = 0; i < message_count; i++)\
{\
SIZE_T message_size = 0;\
m_debugInfo->GetMessage(i, nullptr, &message_size);\
D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(message_size);\
if (message != nullptr)\
{\
m_debugInfo->GetMessage(i, message, &message_size); \
ENGINE_ERROR("Directx11: {} - {}", message->DescriptionByteLength, message->pDescription);\
}\
free(message);\
}\
m_debugInfo->ClearStoredMessages();
