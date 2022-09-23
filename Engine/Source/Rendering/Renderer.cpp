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
	DirectX::XMFLOAT3 camPosition = { 0.0f,0.0f,-5.0f };
	m_editorCamera = std::make_unique<Camera>(camPosition, (float)width, (float)height);
	m_frameBuffer = std::make_unique<FrameBuffer>(width, height, m_direct3d);
}

Engine::Rendering::Renderer::~Renderer()
{
	m_direct3d.reset();
}

const std::unique_ptr<Engine::Rendering::Camera>& Engine::Rendering::Renderer::GetEditorCamera()
{
	return m_editorCamera;
}

const DirectX::XMMATRIX& Engine::Rendering::Renderer::GetProjectionMatrix() const
{
	return m_projMatrix;
}

Engine::Core::SystemType Engine::Rendering::Renderer::GetSystemType()
{
    return Engine::Core::SystemType::Renderer;
	
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
	m_frameBuffer->SetAsRenderTarget();
	for (const auto& drawable : m_drawables)
	{
		drawable->Update();
		auto transform = DirectX::XMMatrixTranspose(drawable->GetTransform() * m_editorCamera->GetViewMatrix() * GetProjectionMatrix());
		m_transformationCBuff->Update(transform);
		drawable->Draw();
	}
	m_direct3d->SetBackBufferRenderTarget();
	m_direct3d->BeginFrame();
	Engine::Gui::GuiManager::DrawEditorGui(this);
	m_direct3d->EndFrame();
	
}

const std::unique_ptr<Engine::Rendering::FrameBuffer>& Engine::Rendering::Renderer::GetFrameBuffer()const
{
	return m_frameBuffer;
}

const std::shared_ptr<Engine::Rendering::Direct3D>& Engine::Rendering::Renderer::GetD3D() const
{
	return m_direct3d;
}
