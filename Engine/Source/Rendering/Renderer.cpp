#include "Renderer.h"
#include <Windows.h>
#include "../Gui/GuiManager.h"
#include "../Core/Events.h"
#include "../Core/Window.h"
#include "../ECS/MeshComponent.h"
#include "../ECS/TransformComponent.h"

Engine::Rendering::Renderer::Renderer(int64_t width, int64_t height, void* handle)
	: m_aspectRatio((float)height/(float)width)
{
	m_direct3d = std::make_shared<Direct3D>((HWND)handle, width, height);
	m_projMatrix = DirectX::XMMatrixPerspectiveLH(1.0f, m_aspectRatio, 0.5f, 100.0f);
	m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
	m_transformationCBuff->Bind();
	DirectX::XMFLOAT3 camPosition = { 0.0f,0.0f,0.0f };
	m_camera = std::make_unique<Camera>(camPosition, (float)width, (float)height);
}

Engine::Rendering::Renderer::~Renderer()
{
	m_direct3d.reset();
}

const DirectX::XMMATRIX& Engine::Rendering::Renderer::GetProjectionMatrix() const
{
	return m_projMatrix;
}

int Engine::Rendering::Renderer::GetSystemType()const
{
    return static_cast<int>(Engine::Core::SystemTypes::Renderer);
	
}

bool Engine::Rendering::Renderer::Init()
{
	auto m_event = DependencyResolver::ResolveDependency<Engine::Core::Events>();
	m_sceneManager = DependencyResolver::ResolveDependency<ProjectManager>();
	

	if (m_event == nullptr)
	{
		
		return false;
	}

	m_event->Subscribe(EventType::WindowResized, [this]() {
		m_direct3d->HandleWindowResize(m_direct3d->GetWindowWidth(), m_direct3d->GetWindowWidth());
		});


	return true;
}

void Engine::Rendering::Renderer::ProcessFrame()
{
	auto& entities = m_sceneManager->GetCurrentScene()->GetEntities();
	if (m_drawables.size() != m_sceneManager->GetCurrentScene()->GetEntities().size())
	{
		m_drawables.clear();
		for (const auto& entity : entities)
		{
			if (entity->HasComponent(Engine::ECS::ComponentTypes::Mesh) 
				&& entity->HasComponent(Engine::ECS::ComponentTypes::Transform))
			{
				std::unique_ptr<Drawable> drawable = std::make_unique<Drawable>(m_direct3d, entity);

				m_drawables.push_back(std::move(drawable));
			}
		}
	}

	m_direct3d->BeginFrame();
	
	for (const auto& drawable : m_drawables)
	{
		drawable->Update();
		auto transform = DirectX::XMMatrixTranspose(drawable->GetTransform() * m_camera->GetViewMatrix() * GetProjectionMatrix());
		m_transformationCBuff->Update(transform);
		drawable->Draw();
	}
	Engine::Gui::GuiManager::DrawEditorGui();
	m_direct3d->EndFrame();
	
}

const std::shared_ptr<Engine::Rendering::Direct3D>& Engine::Rendering::Renderer::GetD3D() const
{
	return m_direct3d;
}
