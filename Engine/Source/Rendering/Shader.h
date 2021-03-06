#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <memory>
#include "Renderer.h"

namespace Engine::Rendering {

	enum class ShaderType {
		Vertex = 1, Pixel = 2, Geometry = 3
	};

	class CHILLI_API Shader {
	public:
		Shader(ShaderType type, const std::wstring& path, std::shared_ptr<Renderer> renderer);
		ID3DBlob* GetByteCode()const;
		void Bind()const;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> m_byteCode;
		HRESULT m_hresult = 0;
		std::shared_ptr<Renderer> m_renderer;
		ShaderType m_type;
	};

}