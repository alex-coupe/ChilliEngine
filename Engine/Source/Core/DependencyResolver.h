#pragma once
#include "ChilliDefinitions.h"
#include <vector>
#include <map>
#include <memory>
#include "SubSystem.h"

namespace Chilli {

	class DependencyResolver {
	public:
		template<typename U>
		static std::shared_ptr<U> ResolveDependency()
		{
			auto itr = m_cache.find(U::GetSystemType());
			if (itr == m_cache.end()) 
			{
				CHILLI_WARN("Failed to resolve dependency");
				return nullptr;
			}
			return std::static_pointer_cast<U>(m_dependencies.at(itr->second));
		}

		static void Add(const std::shared_ptr<SubSystem>& in, SystemType type)
		{
			m_dependencies.push_back(in);
			m_cache.emplace(type, m_dependencies.size() - 1);
		}

		static void Flush()
		{
			CHILLI_INFO("Dependencies flushed");
			m_dependencies = {};
			m_cache = {};
		}
	private:
		static std::vector<std::shared_ptr<SubSystem>> m_dependencies;
		static std::map<SystemType, size_t> m_cache;
	};
}
