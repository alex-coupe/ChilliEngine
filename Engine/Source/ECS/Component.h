#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Gui/GuiManager.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>
#include <sstream>

namespace Engine::ECS {

	enum class ComponentTypes : int {
		Transform=-1, Mesh=0,Camera=1,Light=2,Script=3,BoxCollider2D=4, RigidBody2D = 5,
		CircleCollider= 6, BoxCollider=7,CapsuleCollider=8,MeshCollider=9,
		RigidBody=10,AudioListener=11,AudioSource=12,Sprite=13,ParticleEmitter=14,Animation=15,
		Pathfinding=16,Skybox=17
	};

	struct ComponentVariables {};

	class CHILLI_API Component {
	public:
		Component(ComponentTypes type, const std::string& name) :
			m_type(type), m_name(name) {}
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
	};

}