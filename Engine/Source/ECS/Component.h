#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Gui/GuiManager.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>
#include <sstream>
namespace Engine::ECS {

	enum class ComponentTypes : int {
		Transform=1, Mesh=2
	};

	class CHILLI_API Component {
	public:
		Component(ComponentTypes type, const std::string& name) :m_type(type), m_name(name) {}
		virtual ~Component() {};
		virtual const std::string Serialize()const = 0;
		virtual void DrawGui()const = 0;
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