#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/UUID.h"
#include <filesystem>
#include <sstream>

namespace Chilli {

	enum class AssetType : int {
		Mesh, Scene, Texture, Script, Shader, Audio, Font,Material
	};

	class CHILLI_API Asset {
	public:
		Asset(AssetType type,std::filesystem::path filepath, UUID uuid) 
			:m_type(type)
			,m_filePath(filepath),
			Uuid(uuid)
		{}
		Asset(AssetType type, std::filesystem::path filepath)
			:m_type(type)
			, m_filePath(filepath),
			Uuid()
		{}
		AssetType GetAssetType()const
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
		const AssetType m_type;
	};
}
