#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Gui/GuiManager.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>
#include <sstream>

namespace Engine::ECS {

	enum class ComponentTypes : int {
		Transform=-1, Mesh=0,Camera=1,Light=2,Script=3,BoxCollider2D=4,BoxCollider=5,
		SphereCollider2D=6,SphereCollider=7,MeshCollider2D=8,MeshCollider=9,RigidBody2D=10,
		RigidBody=11,AudioListener=12,AudioSource=13,Sprite=14,ParticleEmitter=15,Animation=16,
		Pathfinding=17,Skybox=18
	};

	struct ComponentVariables {};

	class CHILLI_API Component {
	public:
		Component(ComponentTypes type, const std::string& name, const Engine::Utilities::UUID& uuid) :
			m_type(type), m_name(name), m_owningEntityUuid(uuid) {}
		virtual ~Component() {};
		virtual const std::string Serialize()const = 0;
		ComponentTypes GetComponentType()const
		{
			return m_type;
		}
		const std::string& GetName()const
		{
			return m_name;
		}
		
	protected:
		const std::string m_name;
		const ComponentTypes m_type;
		Engine::Utilities::UUID m_owningEntityUuid;
	};

}