#pragma once
#include "../Core/ChilliDefinitions.h"
#include <random>

namespace Chilli {

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<uint64_t> distribution;
	
	class CHILLI_API UUID {
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& rhs) = default;
		uint64_t Get()const;
	private:
		uint64_t m_uuid;
	};
}

namespace std {
	template <> struct hash<Chilli::UUID>
	{
		size_t operator()(const Chilli::UUID& uuid) const
		{
			return uuid.Get();
		}
	};
}



