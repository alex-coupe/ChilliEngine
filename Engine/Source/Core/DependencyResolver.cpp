#include "DependencyResolver.h"

namespace Chilli {
	std::vector<std::shared_ptr<SubSystem>> DependencyResolver::m_dependencies = {};
	std::map<SystemType, size_t> DependencyResolver::m_cache = {};
}