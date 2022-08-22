#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Rendering/VertexTypes.h"
#include <vector>
#include <iterator>
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

namespace Engine::SceneManagement {

	class CHILLI_API Mesh {
	public:
		Mesh(const std::string& filepath);
		void ProcessSubMesh(aiNode* node, const aiScene* scene);
		const std::string& GetFilePath()const;
		const std::vector<Engine::Rendering::VertexPos>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
	private:
		class SubMesh {
		public:
			SubMesh(aiMesh* mesh, const aiScene* scene);
			const std::vector<Engine::Rendering::VertexPos>& GetVertices()const;
			const std::vector<unsigned short>& GetIndices()const;
		private:
			std::vector<Engine::Rendering::VertexPos> m_vertices;
			std::vector<unsigned short> m_indices;
		};
		std::vector<SubMesh> m_subMeshes;
		std::vector<Engine::Rendering::VertexPos> m_vertices;
		std::vector<unsigned short> m_indices;
		const std::string m_filepath;
	};
}