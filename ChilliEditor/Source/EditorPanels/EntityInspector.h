#pragma once
#include <vector>
#include <string>

namespace Chilli {
	class EntityInspector {
	public:
		void DrawGui();
	private:
		std::vector<std::string> m_tags;
		const char* m_componentsList[18] =
		{ "Mesh","Camera","Light","Script","BoxCollider2D","RigidBody2D","CircleCollider",
			"BoxCollider","CapsuleCollider","MeshCollider","RigidBody","AudioListener","AudioSource",
			"Sprite","ParticleEmitter","Animation",	"Pathfinding","Skybox" };
	};
}