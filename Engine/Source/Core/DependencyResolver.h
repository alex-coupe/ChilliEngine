#pragma once
#include "ChilliDefinitions.h"
#include <vector>
#include <map>
#include <memory>

namespace Engine::Core {

	template <typename T>
	class DependencyResolver {
	public:
		template<typename U>
		std::shared_ptr<U> ResolveDependency()
		{	
			
			for (const auto& item : m_dependencies)
			{
				if (std::shared_ptr<U> dependency = std::dynamic_pointer_cast<U>(item); dependency != nullptr)
					return dependency;
			}

			return nullptr;
		}

		void Add(std::shared_ptr<T> in)
		{
			m_dependencies.push_back(in);
		}

		void Flush()
		{
			m_dependencies = {};
		}
	private:
		std::vector<std::shared_ptr<T>> m_dependencies = {};
		std::map<int, int> m_cache = {};
	};
}