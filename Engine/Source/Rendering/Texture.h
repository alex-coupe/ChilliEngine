#pragma once

#include "../Core/ChilliDefinitions.h"
#include "stb/stb_image.h"
#include <string>

namespace Engine::Rendering {

	class CHILLI_API Texture {
	public:
		Texture(const std::string& path)
		{
			m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_colors, 4);

			if (m_data)
			{
				stbi_image_free(m_data);
			}
		}
	private:
		unsigned char* m_data = nullptr;
		int m_width = 0, m_height = 0, m_colors = 0;
	};
}