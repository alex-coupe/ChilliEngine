#pragma once
#include "../Core/ChilliDefinitions.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>
namespace Engine::ECS {

	enum class ComponentTypes {
		Transform=1, Mesh=2
	};

	class CHILLI_API Component {
	public:
		virtual ~Component() {};
		virtual void Serialize(const std::string& filename) = 0;
		virtual void Deserialize(const std::string& filename) = 0;
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