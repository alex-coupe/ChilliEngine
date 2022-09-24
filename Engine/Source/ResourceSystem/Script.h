#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Asset.h"
#include <string>
#include <fstream>

namespace Chilli {
	class CHILLI_API Script : public Asset {
	public:
		Script(const std::filesystem::path& filepath, UUID uuid);
		Script(const std::filesystem::path& filepath);
		virtual const std::string Serialize()const override;
		char* GetPayload()const;
		uint32_t GetPayloadSize()const;
		~Script();
	private:
		char* ReadBytes(const std::filesystem::path& filepath);
		const std::string m_scriptName = m_filePath.stem().string();
		char* m_payload;
		uint32_t m_payloadSize;
		std::vector<std::string> m_classNames;
	};
}