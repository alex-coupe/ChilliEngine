#include "Script.h"

namespace Chilli {
    Script::Script(const std::filesystem::path& filepath,UUID uuid)
        : Asset(AssetTypes::Script, filepath, uuid)
    {
        m_payload = ReadBytes(filepath);
    }

    Script::Script(const std::filesystem::path& filepath)
        :Asset(AssetTypes::Script, filepath)
    {
        m_payload = ReadBytes(filepath);
    }

    const std::string Script::Serialize() const
    {
        std::stringstream ss;
        ss << "{ \"Uuid\":" << Uuid.Get() << ", \"Type\":" << static_cast<int>(m_type) << ", \"FilePath\": \"Assets/Scripts/" << m_filePath.filename().string() << "\"}";
        return  ss.str();
    }

    char* Script::GetPayload() const
    {
        return m_payload;
    }

    uint32_t Script::GetPayloadSize() const
    {
        return m_payloadSize;
    }

    Script::~Script()
    {
        delete[] m_payload;
    }

    char* Script::ReadBytes(const std::filesystem::path& filepath)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = (uint32_t)end - (uint32_t)stream.tellg();

        if (size == 0)
        {
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        m_payloadSize = size;
        return buffer;
    }
}
