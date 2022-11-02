#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Gui/GuiManager.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>
#include <sstream>

namespace Chilli {

	enum class ComponentType : int {
		ID = -2, Transform, Mesh,Camera,Light,Script,BoxCollider2D, RigidBody2D,
		CircleCollider, BoxCollider,CapsuleCollider,MeshCollider,
		RigidBody,AudioListener,AudioSource,Sprite,ParticleEmitter,Animation,
		Pathfinding,Skybox
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