#pragma once
#include "../Core/ChilliDefinitions.h"
#include <unordered_map>
#include <fstream>
#include "Asset.h"
#include "Mesh.h"
#include "../Rendering/Texture.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace Chilli {

	class CHILLI_API AssetManager
	{
	public:
		AssetManager(const std::string& projDir)
			:m_projDir(projDir)
		{}
		void Reset();
		void LoadAssets(const rapidjson::Value& assets);
		std::string SaveAssets()const;
		void AddMesh(const std::filesystem::path& filename);
		void RemoveMesh(UUID meshUuid);
		void AddTexture(const std::filesystem::path& filename);
		void AddScript(const std::filesystem::path& filename);
		void RemoveTexture(UUID texUuid);
		std::shared_ptr<Mesh> GetMeshByUUID(UUID uuid)const;
		std::shared_ptr<Texture> GetTextureByUUID(UUID uuid)const;
		const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& GetMeshes()const;
		const std::unordered_map<uint64_t, std::shared_ptr<Texture>>& GetTextures()const;
	private:
		std::string m_projDir;
		std::unordered_map<uint64_t, std::shared_ptr<Mesh>> m_meshes;
		std::unordered_map<uint64_t, std::shared_ptr<Texture>> m_textures;
	};
}