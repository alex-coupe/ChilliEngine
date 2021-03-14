#include "Renderer.h"
#include <Windows.h>

#include "../Core/Event.h"
#include "../Core/Window.h"

#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

Engine::Rendering::Renderer::Renderer(const std::shared_ptr<DependencyResolver<SubSystem>>& resolver, int64_t width, int64_t height, HWND handle)
	: SubSystem(resolver)
{
	m_direct3d = std::make_shared<Direct3D>(handle, width, height);
	m_testCube = std::make_shared<Cube>(m_direct3d);
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
	m_event->Subscribe({ EventType::WindowResized },std::bind(&Cube::Rebind, m_testCube));

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
	m_testCube->Draw();
}
