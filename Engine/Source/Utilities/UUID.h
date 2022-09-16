#pragma once
#include "../Core/ChilliDefinitions.h"
#include <random>
#include <string>
#include <sstream>

namespace Engine::Utilities {

	static std::random_device              rd;
	static std::mt19937                    gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	
	class CHILLI_API UUID {
	public:
		UUID();
		UUID(const UUID& copy) = default;
		UUID& operator=(const UUID& uuid);
		UUID(const char* uuid);
		const unsigned int GetUUIDHash()const;
		const std::string& GetUUID()const;
	private:
		std::string m_uuid;
		unsigned int m_hash;
	};
}

inline bool operator==(const Engine::Utilities::UUID lhs, const Engine::Utilities::UUID rhs)
{
	return lhs.GetUUIDHash() == rhs.GetUUIDHash();
}

inline bool operator!=(const Engine::Utilities::UUID lhs, const Engine::Utilities::UUID rhs)
{
	return lhs.GetUUIDHash() != rhs.GetUUIDHash();
}

inline bool operator>=(const Engine::Utilities::UUID lhs, const Engine::Utilities::UUID rhs)
{
	return lhs.GetUUIDHash() >= rhs.GetUUIDHash();
}

inline bool operator<=(const Engine::Utilities::UUID lhs, const Engine::Utilities::UUID rhs)
{
	return lhs.GetUUIDHash() <= rhs.GetUUIDHash();
}

inline bool operator<(const Engine::Utilities::UUID lhs, const Engine::Utilities::UUID rhs)
{
	return lhs.GetUUIDHash() < rhs.GetUUIDHash();
}

inline bool operator>(const Engine::Utilities::UUID lhs, const Engine::Utilities::UUID rhs)
{
	return lhs.GetUUIDHash() > rhs.GetUUIDHash();
}


