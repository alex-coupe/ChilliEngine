#pragma once
#include <vector>
#include <string>
#include "ECS/Component.h"

namespace Chilli {
	class EntityInspector {
	public:
		void DrawGui();
	private:
		void DrawTransformComponentGui(const std::shared_ptr<Component> comp);
		void DrawScriptComponentGui(const std::shared_ptr<Component> comp);
		void DrawMeshComponentGui(const std::shared_ptr<Component> comp);
		void DrawRigidBody2DComponentGui(const std::shared_ptr<Component> comp);
		void DrawCircleColliderComponentGui(const std::shared_ptr<Component> comp);
		void DrawBoxColliderComponentGui(const std::shared_ptr<Component> comp);
		void DrawLightComponentGui(const std::shared_ptr<Component> comp);
		void DrawCameraComponentGui(const std::shared_ptr<Component> comp);
		void DrawSpriteComponentGui(const std::shared_ptr<Component> comp);
		std::vector<std::string> m_tags;
		const char* m_componentsList[18] =
		{ "Mesh","Camera","Light","Script","BoxCollider2D","RigidBody2D","CircleCollider",
			"BoxCollider","CapsuleCollider","MeshCollider","RigidBody","AudioListener","AudioSource",
			"Sprite","ParticleEmitter","Animation",	"Pathfinding","Skybox" };
	};
}