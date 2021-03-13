#include "Shader.h"
#include <cassert>

Engine::Rendering::Shader::Shader(ShaderType type, const std::wstring& path, const std::shared_ptr<Direct3D>& d3d)
	:m_direct3d(d3d), m_type(type)
{
	auto thing = path.c_str();
	switch (type)
	{
	case ShaderType::Pixel:
		if (FAILED(m_hresult = D3DCompileFromFile(path.c_str(), 0, 0, "main", "ps_4_0", 0, 0, &m_byteCode, nullptr)))
		{
			m_direct3d->GetDXError();
		}

		if (FAILED(m_hresult = m_direct3d->GetDevice()->CreatePixelShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_pixelShader)))
		{
			m_direct3d->GetDXError();
		}
		break;
	case ShaderType::Vertex:	
		if (FAILED(m_hresult = D3DCompileFromFile(path.c_str(), 0, 0, "main", "vs_4_0", 0, 0, &m_byteCode, nullptr)))
		{
			m_direct3d->GetDXError();
		}

		if (FAILED(m_hresult = m_direct3d->GetDevice()->CreateVertexShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_vertexShader)))
		{
			m_direct3d->GetDXError();
		}
		break;
	case ShaderType::Geometry:
		if (FAILED(m_hresult = D3DCompileFromFile(path.c_str(), 0, 0, "main", "gs_4_0", 0, 0, &m_byteCode, nullptr)))
		{
			m_direct3d->GetDXError();
		}

		if (FAILED(m_hresult = m_direct3d->GetDevice()->CreateGeometryShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_geometryShader)))
		{
			m_direct3d->GetDXError();
		}
		break;
	}
}

ID3DBlob* Engine::Rendering::Shader::GetByteCode() const
{
	return m_byteCode.Get();
}

void Engine::Rendering::Shader::Bind() const
{
	switch (m_type)
	{
	case ShaderType::Pixel:
		assert(m_pixelShader != nullptr);
		m_direct3d->GetContext()->PSSetShader(m_pixelShader.Get(), 0u, 0u);
		break;
	case ShaderType::Vertex:
		assert(m_vertexShader != nullptr);
		m_direct3d->GetContext()->VSSetShader(m_vertexShader.Get(), 0u, 0u);
		break;
	case ShaderType::Geometry:
		assert(m_geometryShader != nullptr);
		m_direct3d->GetContext()->GSSetShader(m_geometryShader.Get(), 0u, 0u);
		break;
	}
}
