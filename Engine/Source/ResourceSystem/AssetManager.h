#pragma once
#include "../Core/ChilliDefinitions.h"
#include <unordered_map>
#include "Asset.h"
#include "Mesh.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace Chilli {

	class CHILLI_API AssetManager
	{
	public:
		AssetManager() = default;
		void Reset();
		void LoadAssets(const rapidjson::Value& assets);
		std::string SaveAssets()const;
		void AddMesh(const std::filesystem::path& filename);
		void RemoveMesh(UUID meshUuid);
		std::shared_ptr<Mesh> GetMeshByUUID(UUID uuid)const;
		const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& GetMeshes()const;
	private:
		std::unordered_map<uint64_t, std::shared_ptr<Mesh>> m_meshes;
	};
}