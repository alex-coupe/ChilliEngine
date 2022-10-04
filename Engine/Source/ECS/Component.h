#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Gui/GuiManager.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>
#include <sstream>

namespace Chilli {

	enum class ComponentType : int {
		Transform=-1, Mesh=0,Camera=1,Light=2,Script=3,BoxCollider2D=4, RigidBody2D = 5,
		CircleCollider= 6, BoxCollider=7,CapsuleCollider=8,MeshCollider=9,
		RigidBody=10,AudioListener=11,AudioSource=12,Sprite=13,ParticleEmitter=14,Animation=15,
		Pathfinding=16,Skybox=17
	};

	class CHILLI_API Component {
	public:
		Component(ComponentType type, const std::string& name) :
			m_type(type), m_name(name) {}
		virtual ~Component() {};
		Component(Component& rhs) : m_type(rhs.m_type),m_name(rhs.m_name) {};
		virtual const std::string Serialize(uint64_t)const = 0;
		virtual void Clone(const std::shared_ptr<Component>& rhs) = 0;
		ComponentType GetComponentType()const
		{
			return m_type;
		}
		const std::string& GetName()const
		{
			return m_name;
		}
		
	protected:
		const std::string m_name;
		const ComponentType m_type;
	};

}