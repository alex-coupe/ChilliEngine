#pragma once
#include "../Core/ChilliDefinitions.h"
#include "stb/stb_image.h"
#include "Direct3D.h"

namespace Chilli {

	class CHILLI_API Texture : public Asset {
	public:
		Texture(const std::filesystem::path& path);
		Texture(const std::filesystem::path& path, const UUID uuid);
		void Bind(UINT slot = 0u)const;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView()const;
		virtual const std::string Serialize()const override;
	private:
		unsigned char* m_data = nullptr;
		int m_width = 0, m_height = 0, m_colors = 0;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
		HRESULT m_result = 0;
	};
}