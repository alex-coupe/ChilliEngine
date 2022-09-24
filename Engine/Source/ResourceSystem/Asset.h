#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/UUID.h"
#include <filesystem>
#include <sstream>

namespace Chilli {

	enum class AssetTypes : int {
		Mesh = 0, Audio = 1, Material = 2, Script = 3
	};

	class CHILLI_API Asset {
	public:
		Asset(AssetTypes type,std::filesystem::path filepath, UUID uuid) 
			:m_type(type)
			,m_filePath(filepath),
			Uuid(uuid)
		{}
		Asset(AssetTypes type, std::filesystem::path filepath)
			:m_type(type)
			, m_filePath(filepath),
			Uuid()
		{}
		AssetTypes GetAssetType()const
		{
			return m_type;
		}
		const std::filesystem::path& GetFilePath()const
		{
			return m_filePath;
		}
		const UUID Uuid;
		const std::filesystem::path& GetName()const
		{
			return m_filePath;
		}
		virtual ~Asset() {};
		virtual const std::string Serialize()const = 0;
	protected:
		const std::filesystem::path m_filePath;
		const AssetTypes m_type;
	};
}
