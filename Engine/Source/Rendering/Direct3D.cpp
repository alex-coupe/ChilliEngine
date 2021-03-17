#include "Direct3D.h"


Engine::Rendering::Direct3D::Direct3D(HWND handle, int64_t window_width, int64_t window_height, const std::shared_ptr<Gui::GuiManager>& gui_man)
	:  m_handle(handle), m_width(window_width), m_height(window_height), m_gui(gui_man)
{
	SetUpD3D();
}



void Engine::Rendering::Direct3D::BeginFrame()
{
	m_context->ClearRenderTargetView(m_backBuffer.Get(), DirectX::XMVECTORF32{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_context->ClearDepthStencilView(m_depthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Engine::Rendering::Direct3D::EndFrame()
{
	m_gui->Draw();
	m_swapChain->Present(0u, 0u);
}

void Engine::Rendering::Direct3D::Draw(UINT vertexCount, UINT startVertex)const
{
	m_context->Draw(vertexCount, startVertex);
}

void Engine::Rendering::Direct3D::DrawIndexed(UINT count)const
{
	m_context->DrawIndexed(count, 0u, 0u);
}

void Engine::Rendering::Direct3D::HandleWindowResize(const int64_t width, const int64_t height)
{
	if (m_swapChain && (m_width != width || m_height != height) )
	{
		m_width = width;
		m_height = height;

		ShutdownD3D();
		SetUpD3D();
	}
	
}

int64_t Engine::Rendering::Direct3D::GetWindowWidth() const
{
	return m_width;
}

int64_t Engine::Rendering::Direct3D::GetWindowHeight() const
{
	return m_height;
}

Microsoft::WRL::ComPtr<ID3D11Device> Engine::Rendering::Direct3D::GetDevice()
{
	return m_device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Engine::Rendering::Direct3D::GetContext()
{
	return m_context;
}

void Engine::Rendering::Direct3D::SetUpD3D()
{
	assert(m_handle != nullptr);

	DXGI_MODE_DESC buffer_desc = {};

	buffer_desc.Width = static_cast<unsigned int>(m_width);
	buffer_desc.Height = static_cast<unsigned int>(m_height);
	buffer_desc.RefreshRate.Numerator = 60;
	buffer_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swap_chain = {};
	swap_chain.BufferDesc = buffer_desc;
	swap_chain.BufferCount = 1;
	swap_chain.SampleDesc.Count = 1;
	swap_chain.SampleDesc.Quality = 0;
	swap_chain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain.OutputWindow = m_handle;
	swap_chain.Windowed = TRUE;
	swap_chain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	GFX_THROW_ERR(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		NULL, NULL, D3D11_SDK_VERSION, &swap_chain, m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(), NULL, m_context.GetAddressOf()));

	GFX_THROW_ERR(m_device->QueryInterface(__uuidof(ID3D11InfoQueue), &m_debugInfo));

	GFX_THROW_ERR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_bufferTexture));

	GFX_THROW_ERR(m_device->CreateRenderTargetView(m_bufferTexture.Get(), NULL, &m_backBuffer));
	
	D3D11_DEPTH_STENCIL_DESC depth_stencil = {};

	depth_stencil.DepthEnable = TRUE;
	depth_stencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	GFX_THROW_ERR(m_device->CreateDepthStencilState(&depth_stencil, depthStencilState.GetAddressOf()));
	

	m_context->OMSetDepthStencilState(depthStencilState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_texture;
	D3D11_TEXTURE2D_DESC depth_desc = {};
	depth_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_desc.Width = static_cast<unsigned int>(m_width);
	depth_desc.Height = static_cast<unsigned int>(m_height);
	depth_desc.MipLevels = 1u;
	depth_desc.ArraySize = 1u;
	depth_desc.Format = DXGI_FORMAT_D32_FLOAT;
	depth_desc.SampleDesc.Count = 1u;
	depth_desc.SampleDesc.Quality = 0u;

	GFX_THROW_ERR(m_device->CreateTexture2D(&depth_desc, nullptr, depth_texture.GetAddressOf()));
	
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_view = {};
	depth_view.Format = DXGI_FORMAT_D32_FLOAT;
	depth_view.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_view.Texture2D.MipSlice = 0u;

	GFX_THROW_ERR(m_device->CreateDepthStencilView(depth_texture.Get(), &depth_view, m_depthStencil.GetAddressOf()));
	

	//Set the render target view to the back buffer that we created above
	m_context->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthStencil.Get());

	//Create the Viewport
	D3D11_VIEWPORT view_port = {};

	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	view_port.Width = static_cast<float>(m_width);
	view_port.Height = static_cast<float>(m_height);

	//Set the Viewport
	m_context->RSSetViewports(1, &view_port);

	m_gui->InitDxHook(m_device.Get(), m_context.Get());
}

void Engine::Rendering::Direct3D::ShutdownD3D()
{
	m_context->OMSetRenderTargets(0, 0, 0);
	m_swapChain->Release();
	m_device->Release();
	m_context->Release();
	m_depthStencil->Release();
}


