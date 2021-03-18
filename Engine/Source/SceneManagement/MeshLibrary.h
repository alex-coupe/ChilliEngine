#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Rendering/Mesh.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <unordered_map>

namespace Engine::SceneManagement {

	class CHILLI_API MeshLibrary {
	public:
		MeshLibrary(const std::shared_ptr<Engine::Rendering::Direct3D>& d3d);
		MeshLibrary(const rapidjson::Value& meshes, const std::shared_ptr<Engine::Rendering::Direct3D>& d3d);
		void AddMesh(const std::string& name, const std::string& filepath);
		void RemoveMesh(const std::string& name);
		const std::string Serialize()const;
		void DrawGui()const;
	private:
		std::unordered_map<std::string,std::shared_ptr<Engine::Rendering::Mesh>> m_meshes;
	};

}