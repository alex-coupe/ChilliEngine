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
			U type;
			auto itr = m_cache.find(type.GetSystemType());
			if (itr == m_cache.end()) 
			{
				CHILLI_WARN("Failed to resolve dependency");
				return nullptr;
			}
			return std::static_pointer_cast<U>(m_dependencies.at(itr->second));
		}

		static void Add(const std::shared_ptr<SubSystem>& in)
		{
			m_dependencies.push_back(in);
			m_cache.emplace(in->GetSystemType(), m_dependencies.size() - 1);
		}

		static bool Remove(const std::shared_ptr<SubSystem>& in)
		{
			if (m_cache.size() < 1 || m_dependencies.size() < 1)
				return false;

			auto iterator = m_cache.find(in->GetSystemType());

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
			m_cache = {};
		}
	private:
		static std::vector<std::shared_ptr<SubSystem>> m_dependencies;
		static std::map<int, size_t> m_cache;
	};
}
