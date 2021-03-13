#include "Renderer.h"
#include <Windows.h>
#include "VertexTypes.h"

#include "../Core/Event.h"
#include "../Core/Window.h"

#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

Engine::Rendering::Renderer::Renderer(const std::shared_ptr<DependencyResolver<SubSystem>>& resolver, int64_t width, int64_t height, HWND handle)
	: SubSystem(resolver)
{
	m_direct3d = std::make_shared<Direct3D>(handle, width, height);
}

Engine::Rendering::Renderer::~Renderer()
{
	m_direct3d.reset();
}

int Engine::Rendering::Renderer::GetHash()const
{
    return static_cast<int>(Engine::Core::SystemTypes::Renderer);
	
}

bool Engine::Rendering::Renderer::Init()
{
	auto m_event = m_resolver->ResolveDependency<Event>();

	if (m_event == nullptr)
	{
		
		return false;
	}

	m_event->Subscribe({ EventType::WindowResized },
		std::bind(&Direct3D::HandleWindowResize, m_direct3d, std::cref(m_event->GetScreenWidth()), std::cref(m_event->GetScreenHeight())));

	return true;
}

void Engine::Rendering::Renderer::ProcessFrame()
{
	m_direct3d->BeginFrame();
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
	m_direct3d->EndFrame();
	
}

void Engine::Rendering::Renderer::CreateTestCube()
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

	VertexBuffer vertexBuffer(vertices, m_direct3d);
	vertexBuffer.Bind();

	Shader vertexShader(ShaderType::Vertex, L"../Engine/Source/Rendering/shaders/Vertex_Basic.hlsl", m_direct3d);
	vertexShader.Bind();

	Shader pixelShader(ShaderType::Pixel, L"../Engine/Source/Rendering/shaders/Pixel_Basic.hlsl", m_direct3d);
	pixelShader.Bind();

	InputLayout inputLayout(ied, vertexShader.GetByteCode(), m_direct3d);
	inputLayout.Bind();

	Topology topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_direct3d);
	topology.Bind();

	std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4

	};

	IndexBuffer indexBuffer(indices, m_direct3d);
	indexBuffer.Bind();

	float aspectRatio = static_cast<float>(m_direct3d->GetWindowHeight()) /static_cast<float>(m_direct3d->GetWindowWidth());

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.0f)*
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) * 
		DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio,  0.5f, 100.0f)
	);

	ConstantBuffer constantBuffer(ConstantBufferType::Vertex, transform, m_direct3d);
	constantBuffer.Bind();

	m_direct3d->DrawIndexed(indexBuffer.GetCount());
	
}
