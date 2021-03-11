#include "Shader.h"
#include <cassert>

Engine::Rendering::Shader::Shader(ShaderType type, const std::wstring& path, std::shared_ptr<Renderer> renderer)
	:m_renderer(renderer), m_type(type)
{
	switch (type)
	{
	case ShaderType::Pixel:
		if (FAILED(m_hresult = D3DCompileFromFile(path.c_str(), 0, 0, "main", "ps_4_0", 0, 0, &m_byteCode, nullptr)))
		{
			m_renderer->GetDXError();
		}

		if (FAILED(m_hresult = m_renderer->GetDevice()->CreatePixelShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_pixelShader)))
		{
			m_renderer->GetDXError();
		}
		break;
	case ShaderType::Vertex:
		if (FAILED(m_hresult = D3DCompileFromFile(path.c_str(), 0, 0, "main", "vs_4_0", 0, 0, &m_byteCode, nullptr)))
		{
			m_renderer->GetDXError();
		}

		if (FAILED(m_hresult = m_renderer->GetDevice()->CreateVertexShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_vertexShader)))
		{
			m_renderer->GetDXError();
		}
		break;
	case ShaderType::Geometry:
		if (FAILED(m_hresult = D3DCompileFromFile(path.c_str(), 0, 0, "main", "gs_4_0", 0, 0, &m_byteCode, nullptr)))
		{
			m_renderer->GetDXError();
		}

		if (FAILED(m_hresult = m_renderer->GetDevice()->CreateGeometryShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_geometryShader)))
		{
			m_renderer->GetDXError();
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
		m_renderer->GetContext()->PSSetShader(m_pixelShader.Get(), 0u, 0u);
		break;
	case ShaderType::Vertex:
		assert(m_vertexShader != nullptr);
		m_renderer->GetContext()->VSSetShader(m_vertexShader.Get(), 0u, 0u);
		break;
	case ShaderType::Geometry:
		assert(m_geometryShader != nullptr);
		m_renderer->GetContext()->GSSetShader(m_geometryShader.Get(), 0u, 0u);
		break;
	}
}
