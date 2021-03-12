#include "RenderingSystem.h"
#include <Windows.h>
#include "../Core/EventSystem.h"
#include "../Core/Window.h"
#include "VertexTypes.h"
#include "../Core/Logger.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
Engine::Rendering::RenderingSystem::RenderingSystem(std::shared_ptr<DependencyResolver<EngineSystem>> resolver, int64_t width, int64_t height, HWND handle)
	: EngineSystem(resolver)
{
	m_renderer = std::make_shared<Renderer>(handle, width, height);
}

int Engine::Rendering::RenderingSystem::GetHash()
{
    return static_cast<int>(Engine::Core::SystemTypes::Renderer);
	
}

bool Engine::Rendering::RenderingSystem::Init()
{
	auto m_eventSystem = m_resolver->ResolveDependency<EventSystem>();

	if (m_eventSystem == nullptr)
	{
		ENGINE_ERROR("Failed To Resolve Event System Dependency");
		return false;
	}

	m_eventSystem->Subscribe({ EventType::WindowResized },
		std::bind(&Renderer::HandleWindowResize, m_renderer, std::ref(m_eventSystem->m_data.screen_width), std::ref(m_eventSystem->m_data.screen_height)));

	return true;
}

void Engine::Rendering::RenderingSystem::ProcessFrame()
{
	m_renderer->BeginFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{

		ImGui::Begin("Debug");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));

		ImGui::End();

	}
	CreateTestCube();
	m_renderer->EndFrame();
	
}

void Engine::Rendering::RenderingSystem::CreateTestCube()
{
	std::vector<VertexPos> vertices = {
		{{ -1.0f,-1.0f,-1.0f }},
		{{ 1.0f,-1.0f,-1.0f }},
		{{ -1.0f,1.0f,-1.0f }},
		{{ 1.0f,1.0f, -1.0f }},
		{{ -1.0f,-1.0f,1.0f }},
		{{ 1.0f,-1.0f,1.0f }},
		{{ -1.0f,1.0f,1.0f }},
		{{ 1.0f,1.0f,1.0f }},
		
	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	VertexBuffer vertexBuffer(vertices, m_renderer);
	vertexBuffer.Bind();

	Shader vertexShader(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Basic.hlsl", m_renderer);
	vertexShader.Bind();

	Shader pixelShader(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Basic.hlsl", m_renderer);
	pixelShader.Bind();

	InputLayout inputLayout(ied, vertexShader.GetByteCode(), m_renderer);
	inputLayout.Bind();

	Topology topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_renderer);
	topology.Bind();

	std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4

	};

	IndexBuffer indexBuffer(indices, m_renderer);
	indexBuffer.Bind();

	float aspectRatio = static_cast<float>(m_renderer->GetWindowHeight()) /static_cast<float>(m_renderer->GetWindowWidth());

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) * 
		DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio,  0.5f, 100.0f)
	);

	ConstantBuffer constantBuffer(ConstantBufferType::Vertex, transform, m_renderer);
	constantBuffer.Bind();

	m_renderer->DrawIndexed(indexBuffer.GetCount());
	
}
