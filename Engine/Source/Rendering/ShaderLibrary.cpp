#include "ShaderLibrary.h"

namespace Chilli {

	std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::m_coreShaders = {};
	std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::m_customShaders = {};
	std::shared_ptr<Direct3D> ShaderLibrary::m_direct3d = nullptr;

	void ShaderLibrary::Init(const std::shared_ptr<Direct3D>& d3d)
	{
		m_direct3d = d3d;

		m_coreShaders.emplace("VertexTex", std::make_shared<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Texture.hlsl", m_direct3d));
		m_coreShaders.emplace("VertexCol", std::make_shared<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Color.hlsl", m_direct3d));
		m_coreShaders.emplace("VertexPhong", std::make_shared<Shader>(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Phong.hlsl", m_direct3d));
		m_coreShaders.emplace("PixelTex", std::make_shared<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Texture.hlsl", m_direct3d));
		m_coreShaders.emplace("PixelTexTransparent", std::make_shared<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Texture_Transparent.hlsl", m_direct3d));
		m_coreShaders.emplace("PixelBasic", std::make_shared<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Basic.hlsl", m_direct3d));
		m_coreShaders.emplace("PixelCol", std::make_shared<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Color.hlsl", m_direct3d));
		m_coreShaders.emplace("PixelPhongTex", std::make_shared<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Phong.hlsl", m_direct3d));
		m_coreShaders.emplace("PixelPhongCol", std::make_shared<Shader>(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_PhongCol.hlsl", m_direct3d));
	}

	std::shared_ptr<Shader> ShaderLibrary::GetCoreShader(const std::string& shaderName)
	{
		auto shaderItr = m_coreShaders.find(shaderName);
		if (shaderItr != m_coreShaders.end())
			return shaderItr->second;

		return nullptr;
	}

	std::shared_ptr<Shader> ShaderLibrary::GetCustomShader(const std::string& shaderName)
	{
		auto shaderItr = m_customShaders.find(shaderName);
		if (shaderItr != m_customShaders.end())
			return shaderItr->second;
		
		return nullptr;
	}

	void ShaderLibrary::AddCustomShader(const std::filesystem::path shaderPath, ShaderType shaderType)
	{
		m_customShaders.emplace(shaderPath.stem().generic_string(), std::make_shared<Shader>(shaderType, shaderPath.wstring(), m_direct3d));
	}

	void ShaderLibrary::RemoveCustomShader(const std::string& shaderName)
	{
		auto shaderItr = m_customShaders.find(shaderName);
		if (shaderItr != m_customShaders.end())
			 m_customShaders.erase(shaderItr);
	}

	void ShaderLibrary::Shutdown()
	{
		m_direct3d.reset();
		
		m_coreShaders.clear();
		m_customShaders.clear();
	}
}