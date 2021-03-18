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
		Component(ComponentTypes type) :m_type(type) {}
		virtual ~Component() {};
		virtual const std::string Serialize()const = 0;
		virtual void DrawGui()const = 0;
		unsigned int GetComponentType()const
		{
			return static_cast<unsigned int>(m_type);
		}
		std::string GetName()const
		{
			return m_name;
		}
	protected:
		std::string m_name;
		const ComponentTypes m_type;
	};

}