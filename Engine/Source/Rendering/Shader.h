#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Chilli {

	enum class ShaderType {
		Vertex = 1, Pixel = 2, Geometry = 3
	};

	class CHILLI_API Shader {
	public:
		Shader(ShaderType type, const std::wstring& path, const std::shared_ptr<Direct3D>& d3d);
		ID3DBlob* GetByteCode()const;
		void Bind()const;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> m_byteCode;
		Microsoft::WRL::ComPtr<ID3DBlob> m_errors;
		HRESULT m_result = 0;
		std::shared_ptr<Direct3D> m_direct3d;
		ShaderType m_type;
	};

}