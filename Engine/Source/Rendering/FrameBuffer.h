#pragma once
#include "Direct3D.h"
#include "../Core/ChilliDefinitions.h"

namespace Chilli {

	class CHILLI_API FrameBuffer {
	public:
		FrameBuffer(uint64_t width, uint64_t  height, const std::shared_ptr<Direct3D>& d3d);
		void SetAsRenderTarget();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView()const;
	private:
		const std::shared_ptr<Direct3D>& m_d3d;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTargetTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	};

}

