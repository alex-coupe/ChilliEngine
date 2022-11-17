#include "FrameBuffer.h"

namespace Chilli {

	FrameBuffer::FrameBuffer(uint64_t  width, uint64_t  height, const std::shared_ptr<Direct3D>& d3d, UINT format, UINT access)
		:m_d3d(d3d)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = (UINT)width;
		textureDesc.Height = (UINT)height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = (DXGI_FORMAT)format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = access;
		textureDesc.MiscFlags = 0;

		auto result = m_d3d->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
		if (FAILED(result))
			CHILLI_ERROR("Error Creating Texture", result);

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		result = m_d3d->GetDevice()->CreateRenderTargetView(m_renderTargetTexture.Get(), &renderTargetViewDesc, &m_renderTargetView);
		if (FAILED(result))
			CHILLI_ERROR("Error Creating Render Target View", result);

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		result = m_d3d->GetDevice()->CreateShaderResourceView(m_renderTargetTexture.Get(), &shaderResourceViewDesc, &m_shaderResourceView);
		if (FAILED(result))
			CHILLI_ERROR("Error Creating Shader Resource View", result);
	}

	void FrameBuffer::Bind()
	{
		m_d3d->GetContext()->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_d3d->GetDepthStencil().Get());
		m_d3d->GetContext()->ClearRenderTargetView(m_renderTargetView.Get(), DirectX::XMVECTORF32{ 0.0f, 0.48f, 0.64f, 1.0f });
		m_d3d->GetContext()->ClearDepthStencilView(m_d3d->GetDepthStencil().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void FrameBuffer::Unbind()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3d->GetContext()->OMSetRenderTargets(1, nullViews, 0);
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> FrameBuffer::GetShaderResourceView()const
	{
		return m_shaderResourceView;
	}

	FrameBuffer::~FrameBuffer()
	{
		m_renderTargetTexture.Reset();
		m_renderTargetView.Reset();
		m_shaderResourceView.Reset();
	}
}