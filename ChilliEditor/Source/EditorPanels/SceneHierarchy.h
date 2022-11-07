#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"

namespace Chilli {

	class SceneHierarchy  {
	public:
		void DrawGui();
	private:
		void DrawEntityNode(const std::shared_ptr<Entity> entity)const;
	};
}