#include "RenderingSystem.h"
#include <Windows.h>
#include "../Core/EventSystem.h"
#include "VertexTypes.h"

Engine::Rendering::RenderingSystem::RenderingSystem(HWND& handle, int64_t window_width, int64_t window_height, std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver)
	: EngineSystem(m_resolver)
{
	m_renderer = std::make_shared<Renderer>(handle, window_width, window_height);
}

int Engine::Rendering::RenderingSystem::GetHash()
{
    return static_cast<int>(Engine::Core::SystemTypes::Renderer);
	
}

void Engine::Rendering::RenderingSystem::Init()
{
	auto m_eventSystem = m_resolver->ResolveDependency<EventSystem>();
	m_eventSystem->Subscribe({ EventType::WindowResized },
		std::bind(&Renderer::HandleWindowResize, m_renderer, std::ref(m_eventSystem->m_data.screen_width), std::ref(m_eventSystem->m_data.screen_height)));
}

void Engine::Rendering::RenderingSystem::ProcessFrame()
{
	m_renderer->BeginFrame();
	CreateTestTriangle();
	m_renderer->EndFrame();
	
}

void Engine::Rendering::RenderingSystem::CreateTestTriangle()
{
	std::vector<VertexPos> vertices = {
		{{ 0.0f, 0.5f,0.0f}},
		{{ 0.5f, -0.5f,0.0f}},
		{{-0.5f,-0.5f,0.0f}}
		
		
	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	VertexBuffer vertexBuffer(vertices, m_renderer);
	vertexBuffer.Bind();

	Shader vertexShader(ShaderType::Vertex, L"C:\\Users\\Alexander\\Documents\\Repos\\ChilliEngine\\Engine\\Source\\Rendering\\shaders\\Vertex_Basic.hlsl", m_renderer);
	vertexShader.Bind();

	Shader pixelShader(ShaderType::Pixel, L"C:\\Users\\Alexander\\Documents\\Repos\\ChilliEngine\\Engine\\Source\\Rendering\\shaders\\Pixel_Basic.hlsl", m_renderer);
	pixelShader.Bind();

	InputLayout inputLayout(ied, vertexShader.GetByteCode(), m_renderer);
	inputLayout.Bind();

	Topology topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_renderer);
	topology.Bind();

	m_renderer->Draw(3, 0);

	
}
