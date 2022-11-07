#include "Mesh.h"

namespace Chilli {

	Mesh::Mesh(const std::filesystem::path& filepath, UUID uuid)
		: Asset(AssetType::Mesh, std::filesystem::path(filepath), uuid)
	{
		ImportMesh();
	}

	Mesh::Mesh(const std::filesystem::path& filepath)
		: Asset(AssetType::Mesh, std::filesystem::path(filepath))
	{
		ImportMesh();
	}

	const std::string Mesh::Serialize() const
	{
		std::stringstream ss;
		ss << "{ \"Uuid\":" << Uuid.Get() << ", \"Type\":" << static_cast<int>(m_type) << ", \"FilePath\": \"Assets/Meshes/" << m_filePath.filename().string() << "\"}";
		return  ss.str();
	}

	void Mesh::ProcessSubMesh(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_subMeshes.push_back(SubMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessSubMesh(node->mChildren[i], scene);
		}
	}

	const std::vector<VertexPosTexNorm>& Mesh::GetVertices() const
	{
		return m_vertices;
	}

	const std::vector<unsigned short>& Mesh::GetIndices() const
	{
		return m_indices;
	}

	void Mesh::ImportMesh()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_filePath.string(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		ProcessSubMesh(scene->mRootNode, scene);

		for (const auto& subMesh : m_subMeshes)
		{
			int offset = static_cast<unsigned int>(m_vertices.size());

			std::copy(subMesh.GetVertices().cbegin(), subMesh.GetVertices().cend(), std::back_inserter(m_vertices));
			for (auto i : subMesh.GetIndices())
			{
				m_indices.push_back(i + offset);
			}
		}
	}

	Mesh::SubMesh::SubMesh(aiMesh* mesh, const aiScene* scene)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			VertexPosTexNorm vertex = {};

			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.tex.x = mesh->mTextureCoords[0][i].x;
				vertex.tex.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.tex.x = 0.0f;
				vertex.tex.y = 0.0f;
			}
				
			vertex.normals.x = mesh->mNormals[i].x;
			vertex.normals.y = mesh->mNormals[i].y;
			vertex.normals.z = mesh->mNormals[i].z;

			m_vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
				m_indices.push_back(face.mIndices[j]);
		}
	}

	const std::vector<VertexPosTexNorm>& Mesh::SubMesh::GetVertices() const
	{
		return m_vertices;
	}

	const std::vector<unsigned short>& Mesh::SubMesh::GetIndices() const
	{
		return m_indices;
	}
}