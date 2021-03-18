#include "Renderer.h"
#include <Windows.h>

#include "../Core/Event.h"
#include "../Core/Window.h"
#include "../ECS/MeshComponent.h"
#include "../ECS/TransformComponent.h"

Engine::Rendering::Renderer::Renderer(const std::shared_ptr<DependencyResolver<SubSystem>>& resolver, int64_t width, int64_t height, HWND handle, 
	const std::shared_ptr<Engine::Gui::GuiManager>& gui_man)
	: SubSystem(resolver), m_aspectRatio((float)height/(float)width)
{
	m_direct3d = std::make_shared<Direct3D>(handle, width, height, gui_man);
	m_projMatrix = DirectX::XMMatrixPerspectiveLH(1.0f, m_aspectRatio, 0.5f, 100.0f);

}

Engine::Rendering::Renderer::~Renderer()
{
	m_direct3d.reset();
}

const DirectX::XMMATRIX& Engine::Rendering::Renderer::GetProjectionMatrix() const
{
	return m_projMatrix;
}

int Engine::Rendering::Renderer::GetHash()const
{
    return static_cast<int>(Engine::Core::SystemTypes::Renderer);
	
}

bool Engine::Rendering::Renderer::Init()
{
	auto m_event = m_resolver->ResolveDependency<Engine::Core::Event>();

	m_sceneManager = m_resolver->ResolveDependency<SceneManager>();

	auto &entities = m_sceneManager->GetCurrentScene()->GetEntities();

	for (const auto& entity : entities)
	{
		auto transform = std::dynamic_pointer_cast<Engine::ECS::TransformComponent>(entity->GetComponent("Transform"));
		auto mesh = std::dynamic_pointer_cast<Engine::ECS::MeshComponent>(entity->GetComponent("Mesh"));
		auto x = mesh->GetVertices();
		std::unique_ptr<Drawable> drawable = std::make_unique<Drawable>(m_direct3d, x, mesh->GetIndices(), transform->GetTransformMatrix(), m_projMatrix);
		
		m_drawables.push_back(std::move(drawable));
	}

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
	
	for (const auto& drawable : m_drawables)
	{
		drawable->Draw();
	}

	m_direct3d->EndFrame();
	
}

const std::shared_ptr<Engine::Rendering::Direct3D>& Engine::Rendering::Renderer::GetD3D() const
{
	return m_direct3d;
}
