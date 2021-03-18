#include "Renderer.h"
#include <Windows.h>

#include "../Core/Event.h"
#include "../Core/Window.h"


Engine::Rendering::Renderer::Renderer(const std::shared_ptr<DependencyResolver<SubSystem>>& resolver, int64_t width, int64_t height, HWND handle, 
	const std::shared_ptr<Engine::Gui::GuiManager>& gui_man)
	: SubSystem(resolver)
{
	m_direct3d = std::make_shared<Direct3D>(handle, width, height, gui_man);
//_testCube = std::make_shared<Cube>(m_direct3d);
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
	auto m_event = m_resolver->ResolveDependency<Engine::Core::Event>();

	m_sceneManager = m_resolver->ResolveDependency<SceneManager>();

	if (m_event == nullptr)
	{
		
		return false;
	}

//	m_event->Subscribe({ EventType::WindowResized },std::bind(&Direct3D::HandleWindowResize, m_direct3d, std::cref(m_event->GetScreenWidth()), std::cref(m_event->GetScreenHeight())));
//	m_event->Subscribe({ EventType::WindowResized },std::bind(&Cube::Rebind, m_testCube));

	return true;
}

void Engine::Rendering::Renderer::ProcessFrame()
{
	m_direct3d->BeginFrame();
	//CreateTestCube();
	m_direct3d->EndFrame();
	
}

void Engine::Rendering::Renderer::CreateTestCube()
{
	//m_testCube->Draw();
}

const std::shared_ptr<Engine::Rendering::Direct3D>& Engine::Rendering::Renderer::GetD3D() const
{
	return m_direct3d;
}
