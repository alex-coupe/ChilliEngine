#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/UUID.h"
#include "Asset.h"

namespace Chilli {

	struct CHILLI_API Material {
		std::string Name = "";
		alignas(16)float DiffuseColor[3] = { 1.0f,1.0f,1.0f };
		alignas(16)float SpecularColor[3] = { 0.5f,0.5f,0.5f };
		float Shininess = 32.0f;
		UUID DiffuseTexId = 0;
		UUID SpecularTexId = 0;
		UUID Id;
		AssetType Type = AssetType::Material;
		inline const std::string Serialize()const
		{
			std::stringstream ss;
			ss << "{ \"Type\":" << static_cast<int>(Type) << ", \"Name\":\"" << Name 
				<< "\", \"DiffTexUuid\":" << DiffuseTexId.Get() << ", \"SpecTexUuid\":" << SpecularTexId.Get()
				<< ", \"DiffR\":" << DiffuseColor[0] << ", \"DiffG\":" << DiffuseColor[1] << ", \"DiffB\":" << DiffuseColor[2]
				<< ", \"SpecR\":" << SpecularColor[0] << ", \"SpecG\":" << SpecularColor[1] << ", \"SpecB\":" << SpecularColor[2]
				<< ", \"Shininess\":" << Shininess << ",\"Uuid\":" << Id.Get()
				<< "}";

			return  ss.str();
		}
	};
}