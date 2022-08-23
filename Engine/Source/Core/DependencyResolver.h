#pragma once
#include "ChilliDefinitions.h"
#include <vector>
#include <map>
#include <memory>
#include "SubSystem.h"

namespace Engine::Core {

	class DependencyResolver {
	public:
		template<typename U>
		static std::shared_ptr<U> ResolveDependency()
		{	
			for (auto& item : m_dependencies)
			{
				if (std::shared_ptr<U> dependency = std::dynamic_pointer_cast<U>(item); dependency != nullptr)
					return dependency;
			}
			CHILLI_WARN("Failed to resolve dependency {}");
			return nullptr;
		}

		template<typename U>
		static std::shared_ptr<U> ResolveDependency(const char* name)
		{
			for (auto& item : m_dependencies)
			{
				if (std::shared_ptr<U> dependency = std::dynamic_pointer_cast<U>(item); dependency != nullptr)
					return dependency;
			}
			CHILLI_WARN("Failed to resolve dependency {}", name);
			return nullptr;
		}

		static void Add(const std::shared_ptr<SubSystem>& in)
		{
			m_dependencies.push_back(in);
		}

		static void Add(const std::shared_ptr<SubSystem>& in, const char* name)
		{
			m_dependencies.push_back(in);
			m_cache.emplace( name,m_dependencies.size() - 1 );
		}

		static bool Remove(const char* name)
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

		static void Flush()
		{
			CHILLI_INFO("Dependencies flushed");
			m_dependencies = {};
		}
	private:
		static std::vector<std::shared_ptr<SubSystem>> m_dependencies;
		static std::map<const char*, size_t> m_cache;
	};
}
