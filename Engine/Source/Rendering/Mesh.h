#pragma once
#include "../Core/ChilliDefinitions.h"
#include "VertexTypes.h"
#include "Direct3D.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "IndexBuffer.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

namespace Engine::Rendering {

	class CHILLI_API Mesh {
	public:
		Mesh(const std::string& filepath, const std::shared_ptr<Direct3D>& d3d);
		void Draw()const;
		void ProcessSubMesh(aiNode* node, const aiScene* scene);
	private:
		class SubMesh {
		public:
			SubMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Direct3D>& d3d);
			const std::vector<VertexPos>& GetVertices()const;
			const std::vector<unsigned short>& GetIndices()const;
		private:
			std::vector<VertexPos> m_vertices;
			std::vector<unsigned short> m_indices;
			const std::shared_ptr<Direct3D>& m_direct3d;
		};
		std::vector<SubMesh> m_subMeshes;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::string m_filepath;
		const std::shared_ptr<Direct3D>& m_direct3d;
	};
}