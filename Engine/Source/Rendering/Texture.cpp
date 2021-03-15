#include "Texture.h"

Engine::Rendering::Texture::Texture(const std::string& path, const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d)
{
	m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_colors,4 );


	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = m_width;
	texture_desc.Height = m_height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sub_resource = {};
	sub_resource.pSysMem = m_data;
	sub_resource.SysMemPitch = m_width * m_colors;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

	GFX_THROW_ERR(m_direct3d->GetDevice()->CreateTexture2D(&texture_desc, &sub_resource, texture.GetAddressOf()));
	
	D3D11_SHADER_RESOURCE_VIEW_DESC resource_view = {};
	resource_view.Format = texture_desc.Format;
	resource_view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_view.Texture2D.MostDetailedMip = 0;
	resource_view.Texture2D.MipLevels = 1;
	GFX_THROW_ERR(m_direct3d->GetDevice()->CreateShaderResourceView(texture.Get(), &resource_view, &m_textureView));

	if (m_data)
	{
		stbi_image_free(m_data);
	}

}

void Engine::Rendering::Texture::Bind(UINT slot) const
{
	m_direct3d->GetContext()->PSSetShaderResources(slot, 1u, m_textureView.GetAddressOf());
}
