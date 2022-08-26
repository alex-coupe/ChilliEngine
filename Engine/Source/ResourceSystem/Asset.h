#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Utilities/UUID.h"
#include <filesystem>

namespace Engine::ResourceSystem {

	enum class AssetTypes : int {
		Mesh = 1, Sound = 2, Surface = 3, Script = 4
	};

	class CHILLI_API Asset {
	public:
		Asset(AssetTypes type,std::filesystem::path filepath, Engine::Utilities::UUID uuid, 
			const std::string& name) 
			:m_type(type)
			,m_filePath(filepath),
			m_uuid(uuid),
			m_name(name)
		{}
		AssetTypes GetAssetType()const
		{
			return m_type;
		}
		const std::filesystem::path& GetFilePath()const
		{
			return m_filePath;
		}
		const Engine::Utilities::UUID& GetUUID()const
		{
			return m_uuid;
		}
		virtual ~Asset() {};
		virtual const std::string Serialize()const = 0;
	protected:
		Engine::Utilities::UUID m_uuid;
		const std::filesystem::path m_filePath;
		const AssetTypes m_type;
		const std::string m_name;
	};
}
