#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Rendering/VertexTypes.h"
#include <vector>
#include <iterator>
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "Asset.h"

namespace Chilli {

	class CHILLI_API Mesh : public Asset {
	public:
		Mesh(const std::filesystem::path& filepath, const UUID uuid);
		Mesh(const std::filesystem::path& filepath);
		void ProcessSubMesh(aiNode* node, const aiScene* scene);
		virtual const std::string Serialize()const override;
		const std::vector<VertexPos>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		
	private:
		void ImportMesh();
		class SubMesh {
		public:
			SubMesh(aiMesh* mesh, const aiScene* scene);
			const std::vector<VertexPos>& GetVertices()const;
			const std::vector<unsigned short>& GetIndices()const;
		private:
			std::vector<VertexPos> m_vertices;
			std::vector<unsigned short> m_indices;
		};
		std::vector<SubMesh> m_subMeshes;
		std::vector<VertexPos> m_vertices;
		std::vector<unsigned short> m_indices;
	};
}