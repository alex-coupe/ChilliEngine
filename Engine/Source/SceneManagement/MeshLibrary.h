#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Mesh.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <unordered_map>
#include <memory>
#include <sstream>
#include "../Gui/GuiManager.h"

namespace Engine::SceneManagement {

	class CHILLI_API MeshLibrary {
	public:
		MeshLibrary() = default;
		MeshLibrary(const rapidjson::Value& meshes);
		void AddMesh(const std::string& name, const std::string& filepath);
		void RemoveMesh(const std::string& name);
		const std::string Serialize()const;
		void DrawGui();
	private:
		std::unordered_map<std::string,std::shared_ptr<Mesh>> m_meshes;
	};

}