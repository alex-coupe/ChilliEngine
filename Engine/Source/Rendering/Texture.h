#pragma once
#include "../Core/ChilliDefinitions.h"
#include "stb/stb_image.h"
#include "Direct3D.h"

namespace Chilli {

	class CHILLI_API Texture {
	public:
		Texture(const std::string& path, const std::shared_ptr<Direct3D>& d3d);
		void Bind(UINT slot = 0u)const;
	private:
		unsigned char* m_data = nullptr;
		int m_width = 0, m_height = 0, m_colors = 0;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
		std::shared_ptr<Direct3D> m_direct3d;
		HRESULT m_result = 0;
	};
}