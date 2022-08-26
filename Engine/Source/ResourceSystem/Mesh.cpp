#include "Mesh.h"

Engine::ResourceSystem::Mesh::Mesh(const std::string& filepath)
	: Asset(Engine::ResourceSystem::AssetTypes::Mesh, std::filesystem::path(filepath), Engine::Utilities::UUID())
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_filePath.string(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	ProcessSubMesh(scene->mRootNode,scene);

	for(const auto& subMesh : m_subMeshes)
	{
		int offset = static_cast<unsigned int>(m_vertices.size());
		
		std::copy(subMesh.GetVertices().cbegin(), subMesh.GetVertices().cend(), std::back_inserter(m_vertices));
		for (auto i : subMesh.GetIndices())
		{
			m_indices.push_back(i + offset);
		}
	}
}

const std::string Engine::ResourceSystem::Mesh::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"UUID\":\"" << m_uuid.GetUUID() << "\", \"Type\":" << static_cast<int>(m_type) << ", \"FilePath\":\"" << m_filePath << "\"}";
	return  ss.str();
}

void Engine::ResourceSystem::Mesh::ProcessSubMesh(aiNode* node, const aiScene* scene)
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
	
const std::vector<Engine::Rendering::VertexPos>& Engine::ResourceSystem::Mesh::GetVertices() const
{
	return m_vertices;
}

const std::vector<unsigned short>& Engine::ResourceSystem::Mesh::GetIndices() const
{
	return m_indices;
}

Engine::ResourceSystem::Mesh::SubMesh::SubMesh(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Engine::Rendering::VertexPos vertex = {};

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

const std::vector<Engine::Rendering::VertexPos>& Engine::ResourceSystem::Mesh::SubMesh::GetVertices() const
{
	return m_vertices;
}

const std::vector<unsigned short>& Engine::ResourceSystem::Mesh::SubMesh::GetIndices() const
{
	return m_indices;
}
