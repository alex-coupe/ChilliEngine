#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
namespace Engine::ECS {

	class CHILLI_API Component {
	public:
		virtual ~Component() {};
		virtual void Serialize(const std::string& filename) = 0;
		virtual void Deserialize(const std::string& filename) = 0;
		virtual void DrawGui()const = 0;
		virtual std::string GetName()const
		{
			return m_name;
		}
	protected:
		mutable std::string m_name;
		const unsigned int m_id;
	};

}