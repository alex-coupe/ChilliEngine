#include "Texture.h"
#include "Renderer.h"

namespace Chilli {
	Texture::Texture(const std::filesystem::path& path)
		:Asset(AssetType::Texture, path)
	{
		auto& direct3d = DependencyResolver::ResolveDependency<Renderer>()->GetD3D();
	
		m_data = stbi_load(path.generic_string().c_str(), &m_width, &m_height, &m_colors, 4);

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

		GFX_THROW_ERR(direct3d->GetDevice()->CreateTexture2D(&texture_desc, &sub_resource, texture.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC resource_view = {};
		resource_view.Format = texture_desc.Format;
		resource_view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource_view.Texture2D.MostDetailedMip = 0;
		resource_view.Texture2D.MipLevels = 1;
		GFX_THROW_ERR(direct3d->GetDevice()->CreateShaderResourceView(texture.Get(), &resource_view, &m_textureView));

		if (m_data)
		{
			stbi_image_free(m_data);
		}

	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetShaderResourceView()const
	{
		return m_textureView;
	}

	Texture::Texture(const std::filesystem::path& path, const UUID uuid)
		:Asset(AssetType::Texture, path, uuid)
	{
		auto& direct3d = DependencyResolver::ResolveDependency<Renderer>()->GetD3D();
		m_data = stbi_load(path.string().c_str(), &m_width, &m_height, &m_colors, 4);

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

		GFX_THROW_ERR(direct3d->GetDevice()->CreateTexture2D(&texture_desc, &sub_resource, texture.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC resource_view = {};
		resource_view.Format = texture_desc.Format;
		resource_view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource_view.Texture2D.MostDetailedMip = 0;
		resource_view.Texture2D.MipLevels = 1;
		GFX_THROW_ERR(direct3d->GetDevice()->CreateShaderResourceView(texture.Get(), &resource_view, &m_textureView));

		if (m_data)
		{
			stbi_image_free(m_data);
		}
	}

	void Texture::Bind(UINT slot) const
	{
		auto direct3d = DependencyResolver::ResolveDependency<Renderer>()->GetD3D();
		direct3d->GetContext()->PSSetShaderResources(slot, 1u, m_textureView.GetAddressOf());
	}

	const std::string Texture::Serialize() const
	{
		std::stringstream ss;
		ss << "{ \"Uuid\":" << Uuid.Get() << ", \"Type\":" << static_cast<int>(m_type) << ", \"FilePath\": \"Assets/Textures/" << m_filePath.filename().string() << "\"}";
		return  ss.str();
	}
}