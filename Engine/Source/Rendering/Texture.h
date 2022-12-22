#pragma once
#include "../Core/ChilliDefinitions.h"
#include "DirectXTex.h"
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
		
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
		HRESULT m_result = 0;
	};
}