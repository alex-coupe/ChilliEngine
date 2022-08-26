#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Utilities/UUID.h"
#include <filesystem>

namespace Engine::ResourceSystem {

	enum class AssetTypes : int {
		Mesh = 1, Sound = 2, Surface = 3
	};

	class CHILLI_API Asset {
	public:
		Asset(AssetTypes type,std::filesystem::path filepath, Engine::Utilities::UUID uuid) 
			:m_type(type)
			,m_filePath(filepath),
			m_uuid(uuid)
		{}
		AssetTypes GetAssetType()const
		{
			return m_type;
		}
		const std::filesystem::path& GetFilePath()const
		{
			return m_filePath;
		}
		virtual ~Asset() {};
		virtual const std::string Serialize()const = 0;
	protected:
		Engine::Utilities::UUID m_uuid;
		const std::filesystem::path m_filePath;
		const AssetTypes m_type;
	};
}