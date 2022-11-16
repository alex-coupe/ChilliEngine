#pragma once
#include "../Core/ChilliDefinitions.h"
#include <unordered_map>
#include <fstream>
#include "Asset.h"
#include "Mesh.h"
#include "Material.h"
#include "../Rendering/Texture.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace Chilli {

	class CHILLI_API AssetManager
	{
	public:
		AssetManager(const std::string& projDir)
			:m_projDir(projDir)
		{
			AddDefaultMaterial();
		}
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
		const std::unordered_map<uint64_t, Material>& GetMaterials()const;
		Material& GetMaterial(uint64_t materialId);
		void AddMaterial(Material mat);
		void EditMaterial(Material mat);
		void RemoveMaterial(const UUID matId);
	private:
		void AddDefaultMaterial();
		std::string m_projDir;
		std::unordered_map<uint64_t, std::shared_ptr<Mesh>> m_meshes;
		std::unordered_map<uint64_t, std::shared_ptr<Texture>> m_textures;
		std::unordered_map<uint64_t, Material> m_materials;
	};
}