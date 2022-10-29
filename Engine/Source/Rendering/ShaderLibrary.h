#pragma once
#include "Shader.h"

namespace Chilli {

	class CHILLI_API ShaderLibrary {
	public:
		static void Init(const std::shared_ptr<Direct3D>& d3d);
		static std::shared_ptr<Shader> GetCoreShader(const std::string& shaderName);
		static std::shared_ptr<Shader> GetCustomShader(const std::string& shaderName);
		static void AddCustomShader(const std::filesystem::path shaderPath, ShaderType shaderType);
		static void RemoveCustomShader(const std::string& shaderName);
		static void Shutdown();
	private:
		static std::shared_ptr<Direct3D> m_direct3d;
		static std::unordered_map<std::string, std::shared_ptr<Shader>> m_coreShaders;
		static std::unordered_map<std::string, std::shared_ptr<Shader>> m_customShaders;
	};
}