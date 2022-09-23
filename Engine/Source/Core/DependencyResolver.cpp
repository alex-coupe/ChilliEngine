#include "DependencyResolver.h"

std::vector<std::shared_ptr<Engine::Core::SubSystem>> Engine::Core::DependencyResolver::m_dependencies = {};
std::map<Engine::Core::SystemType, size_t> Engine::Core::DependencyResolver::m_cache = {};