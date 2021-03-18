#include "Mesh.h"

Engine::SceneManagement::Mesh::Mesh(const std::string& filepath)
	: m_filepath(filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	ProcessSubMesh(scene->mRootNode,scene);

	std::vector<Engine::Rendering::VertexPos> vertices;
	std::vector<unsigned short> indices;

	for(const auto& subMesh : m_subMeshes)
	{
		int offset = static_cast<unsigned int>(vertices.size());
		
		std::copy(subMesh.GetVertices().cbegin(), subMesh.GetVertices().cend(), std::back_inserter(vertices));
		for (auto i : subMesh.GetIndices())
		{
			indices.push_back(i + offset);
		}
	}
}

void Engine::SceneManagement::Mesh::ProcessSubMesh(aiNode* node, const aiScene* scene)
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

const std::string& Engine::SceneManagement::Mesh::GetFilePath() const
{
	return m_filepath;
}

Engine::SceneManagement::Mesh::SubMesh::SubMesh(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Engine::Rendering::VertexPos vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		m_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			m_indices.push_back(face.mIndices[j]);
	}
}

const std::vector<Engine::Rendering::VertexPos>& Engine::SceneManagement::Mesh::SubMesh::GetVertices() const
{
	return m_vertices;
}

const std::vector<unsigned short>& Engine::SceneManagement::Mesh::SubMesh::GetIndices() const
{
	return m_indices;
}
