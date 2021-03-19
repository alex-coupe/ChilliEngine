#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Gui/GuiManager.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <assert.h>
#include "../Core/Event.h"
#include <stdexcept>
#include <sstream>
#include <memory>
#include <vector>

#pragma warning(disable:4251)
namespace Engine::Rendering {

#pragma comment (lib, "d3d11.lib") 
#pragma comment (lib, "D3DCompiler.lib")


	class CHILLI_API Direct3D  {
	public:
		Direct3D(HWND handle, int64_t window_width, int64_t window_height, const std::shared_ptr<Engine::Gui::GuiManager>& gui_man);
		Direct3D(const Direct3D&) = delete;
		Direct3D& operator=(const Direct3D&) = delete;
		Direct3D(Direct3D&&)noexcept = default;
		void BeginFrame();
		void EndFrame();
		void Draw(UINT vertexCount, UINT startVertex)const;
		void DrawIndexed(UINT count)const;
		void HandleWindowResize(const int64_t width, const int64_t height);
		int64_t GetWindowWidth()const;
		int64_t GetWindowHeight()const;
		Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	private:
		void SetUpD3D();
		void ShutdownD3D();
		
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBuffer;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_bufferTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D11InfoQueue> m_debugInfo;
		HRESULT m_result = 0;
		HWND m_handle;
		int64_t m_width;
		int64_t m_height;
	};

}

#define GFX_THROW_ERR(call) if (FAILED(m_result = call))\
{\
std::stringstream ss;\
ss << "DirectX Error: " << m_result;\
ss << "Line: " << __LINE__;\
ss << "File" << __FILE__;\
throw std::runtime_error(ss.str());\
}



