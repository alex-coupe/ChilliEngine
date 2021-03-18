#include "Mesh.h"

Engine::Rendering::Mesh::Mesh(const std::string& filepath, const std::shared_ptr<Direct3D>& d3d)
	: m_filepath(filepath), m_direct3d(d3d)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	ProcessSubMesh(scene->mRootNode,scene);

	std::vector<VertexPos> vertices;
	std::vector<unsigned short> indices;

	for(const auto& subMesh : m_subMeshes)
	{
		int offset = static_cast<UINT>(vertices.size());
		
		std::copy(subMesh.GetVertices().cbegin(), subMesh.GetVertices().cend(), std::back_inserter(vertices));
		for (auto i : subMesh.GetIndices())
		{
			indices.push_back(i + offset);
		}
	}
		
	
	m_indexBuffer = std::make_unique<IndexBuffer>(indices, m_direct3d);
	m_indexBuffer->Bind();
	m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, m_direct3d);
	m_vertexBuffer->Bind();
}

Engine::Rendering::Mesh::~Mesh()
{
	m_subMeshes.clear();
	m_vertexBuffer.reset();
	m_direct3d.reset();
	m_indexBuffer.reset();
}

void Engine::Rendering::Mesh::Draw() const
{
	m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
}

void Engine::Rendering::Mesh::ProcessSubMesh(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_subMeshes.push_back(SubMesh(mesh, scene,m_direct3d));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessSubMesh(node->mChildren[i], scene);
	}
}

const std::string& Engine::Rendering::Mesh::GetFilePath() const
{
	return m_filepath;
}

Engine::Rendering::Mesh::SubMesh::SubMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d)
{
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VertexPos vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		m_vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			m_indices.push_back(face.mIndices[j]);
	}
}

Engine::Rendering::Mesh::SubMesh::~SubMesh()
{
	m_direct3d.reset();
}


const std::vector<Engine::Rendering::VertexPos>& Engine::Rendering::Mesh::SubMesh::GetVertices() const
{
	return m_vertices;
}

const std::vector<unsigned short>& Engine::Rendering::Mesh::SubMesh::GetIndices() const
{
	return m_indices;
}
