#include "MeshLibrary.h"


Engine::SceneManagement::MeshLibrary::MeshLibrary(const rapidjson::Value& meshes)
{
    for (unsigned int i = 0; i < meshes.Size(); i++)
    {
        m_meshes.emplace(meshes[i]["Name"].GetString(), std::make_shared<Mesh>(meshes[i]["Filepath"].GetString()));
    }
}

void Engine::SceneManagement::MeshLibrary::AddMesh(const std::string& name, const std::string& filepath)
{
    m_meshes.emplace(name, std::make_shared<Mesh>(filepath));
}

void Engine::SceneManagement::MeshLibrary::RemoveMesh(const std::string& name)
{
    m_meshes.erase(name);
}

const std::string Engine::SceneManagement::MeshLibrary::Serialize() const
{
    std::stringstream ss;
    for (const auto& item : m_meshes)
    {
        ss << "{ \"Filepath\":\"" << item.second->GetFilePath() << "\", \"Name\":"<< item.first << "},";
    }

    //remove the last comma
    auto finalString = ss.str().substr(0, ss.str().size() - 1);
    return  finalString;
}

void Engine::SceneManagement::MeshLibrary::DrawGui()
{
    ImGui::Begin("Meshes");
    for (const auto& item : m_meshes)
    {
        ImGui::Text(item.first.c_str());
       
    }
    ImGui::End();
}
