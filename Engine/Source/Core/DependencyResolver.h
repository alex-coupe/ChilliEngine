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

		template<typename U>
		std::shared_ptr<U> ResolveDependency(const char* name)
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

		void Add(std::shared_ptr<T> in, const char* name)
		{
			m_dependencies.push_back(in);
			m_cache.emplace( name,m_dependencies.size() - 1 );
		}

		bool Remove(const char* name)
		{
			if (m_cache.size() < 1 || m_dependencies.size() < 1)
				return false;

			auto iterator = m_cache.find(name);

			if (iterator != m_cache.end())
			{
				auto index = iterator->second;

				m_dependencies.erase(m_dependencies.begin() + index);
				m_cache.erase(iterator);
				return true;
			}
			return false;
		}

		void Flush()
		{
			m_dependencies = {};
		}
	private:
		std::vector<std::shared_ptr<T>> m_dependencies = {};
		std::map<const char*, size_t> m_cache = {};
	};
}