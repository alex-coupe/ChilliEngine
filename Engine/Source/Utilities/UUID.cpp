#include "UUID.h"
#include "Utils.h"

Engine::Utilities::UUID::UUID()
{
    std::stringstream ss;
    int i;
    ss << std::hex;

    for (i = 0; i < 8; i++) {
        auto rand = dis(gen);
        ss << rand;
    }

    ss << "-";

    for (i = 0; i < 4; i++) {
        auto rand = dis(gen);
        ss << rand;
    }

    ss << "-4";
    for (i = 0; i < 3; i++) {
        auto rand = dis(gen);
        ss << rand;
    }
    ss << "-";
   
    for (i = 0; i < 3; i++) {
        auto rand = dis(gen);
        ss << rand;
    }

    ss << "-";
    for (i = 0; i < 12; i++) {
        auto rand = dis(gen);
        ss << rand;
    };
    m_uuid = ss.str();

    std::vector<std::string> parts = Split(m_uuid, '-');
    int hash = 0;
    for (int i = 0; i < parts.size(); i++)
    {
        hash += StringToInt(parts[i]);
    }
    m_hash = hash += (int)parts.size();
}   

Engine::Utilities::UUID::UUID(const char* uuid)
    :m_uuid(uuid)
{
    std::vector<std::string> parts = Split(uuid, '-');
    int hash = 0;
    for (int i = 0; i< parts.size();i++)
    {
        hash+= StringToInt(parts[i]);
    }
    m_hash = hash+=(int)parts.size();
}

const std::string& Engine::Utilities::UUID::GetUUID() const    
{
    return m_uuid;
}

const unsigned int Engine::Utilities::UUID::GetUUIDHash() const
{
    return m_hash;
}
