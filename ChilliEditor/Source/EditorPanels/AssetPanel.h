#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"

namespace Chilli {
	class AssetPanel {
	public:
		AssetPanel();
		void DrawGui();
	private:
		std::unique_ptr<Texture> m_fileTexture;
		std::unique_ptr<Texture> m_folderTexture;
		std::unique_ptr<Texture> m_plusIcon;
	};
}