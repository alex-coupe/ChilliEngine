#include "UUID.h"

namespace Chilli {

	UUID::UUID()
		:m_uuid(distribution(gen))
	{}

	UUID::UUID(uint64_t uuid)
		: m_uuid(uuid)
	{}

	uint64_t UUID::Get() const
	{
		return m_uuid;
	}
}