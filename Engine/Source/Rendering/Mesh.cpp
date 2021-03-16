#include "Mesh.h"

Engine::Rendering::Mesh::Mesh(const std::string& filepath, const std::shared_ptr<Direct3D>& d3d)
	: m_filepath(filepath), m_direct3d(d3d)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_filepath, aiProcess_Triangulate);

	ProcessSubMesh(scene->mRootNode,scene);

	std::vector<VertexPos> vertices;
	std::vector<unsigned short> indices;

	for(const auto& subMesh : m_subMeshes)
	{
		int offset = vertices.size();
		
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

void Engine::Rendering::Mesh::Draw() const
{
	m_direct3d->DrawIndexed(m_indexBuffer->GetCount());
}

void Engine::Rendering::Mesh::ProcessSubMesh(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_subMeshes.emplace_back(SubMesh(mesh, scene,m_direct3d));
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessSubMesh(node->mChildren[i], scene);
	}
}

Engine::Rendering::Mesh::SubMesh::SubMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        m_vertices.push_back({ { mesh->mVertices[i].x,mesh->mVertices[i].y, mesh->mVertices[i].z } });
    }
   
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
       
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            m_indices.push_back(face.mIndices[j]);
    }
}


const std::vector<Engine::Rendering::VertexPos>& Engine::Rendering::Mesh::SubMesh::GetVertices() const
{
	return m_vertices;
}

const std::vector<unsigned short>& Engine::Rendering::Mesh::SubMesh::GetIndices() const
{
	return m_indices;
}
