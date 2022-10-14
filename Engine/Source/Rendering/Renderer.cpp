#include "Renderer.h"
#include <Windows.h>
#include "../Gui/GuiManager.h"
#include "../Core/Events.h"
#include "../Core/Window.h"
#include "../ECS/MeshComponent.h"
#include "../ECS/TransformComponent.h"

namespace Chilli {

	Renderer::Renderer(int64_t width, int64_t height, void* handle)
		: m_aspectRatio((float)height / (float)width)
	{
		m_direct3d = std::make_shared<Direct3D>((HWND)handle, width, height);
		m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
		m_transformationCBuff->Bind();
		m_editorCamera = std::make_unique<EditorCamera>(1.0f, m_aspectRatio, 0.5f, 100.0f);
		m_frameBuffer = std::make_unique<FrameBuffer>(width, height, m_direct3d);
	}

	Renderer::~Renderer()
	{
		m_frameBuffer.reset();
		m_editorCamera.reset();
		m_drawables.clear();
		m_direct3d.reset();
	}

	const std::unique_ptr<EditorCamera>& Renderer::GetEditorCamera()
	{
		return m_editorCamera;
	}

	SystemType Renderer::GetSystemType()
	{
		return SystemType::Renderer;

	}
	

	bool Renderer::Init()
	{
		auto m_event = DependencyResolver::ResolveDependency<Events>();
		m_sceneManager = DependencyResolver::ResolveDependency<ProjectManager>();

		if (m_event == nullptr)
		{

			return false;
		}

		m_event->Subscribe(EventType::WindowResized, [m_event,this]() {
			HandleResize(m_event->GetScreenWidth(), m_event->GetScreenHeight());
			});

		return true;
	}

	void Renderer::HandleResize(int64_t width, int64_t height)
	{
		if (width == 0 || height == 0)
			return;
		m_frameBuffer.reset();
		m_direct3d->HandleWindowResize(width, height);
		m_frameBuffer = std::make_unique<FrameBuffer>(width, height, m_direct3d);
	}

	void Renderer::ProcessFrame()
	{
		auto& entities = m_sceneManager->GetCurrentScene()->GetEntities();
		if (m_drawables.size() != m_sceneManager->GetCurrentScene()->GetEntities().size())
		{
			m_drawables.clear();
			for (const auto& entity : entities)
			{
				if (entity->HasComponent(ComponentType::Mesh)
					&& entity->HasComponent(ComponentType::Transform))
				{
					std::unique_ptr<Drawable> drawable = std::make_unique<Drawable>(m_direct3d, entity);

					m_drawables.push_back(std::move(drawable));
				}
			}
		}
		m_frameBuffer->Bind();
		for (const auto& drawable : m_drawables)
		{
			drawable->Update();
			auto transform = DirectX::XMMatrixTranspose(drawable->GetTransform() * m_editorCamera->GetViewProjMatrix());
			m_transformationCBuff->Update(transform);
			drawable->Draw();
		}
		m_direct3d->SetBackBufferRenderTarget();
		m_direct3d->BeginFrame();
		GuiManager::DrawEditorGui(this);
		m_direct3d->EndFrame();

	}

	const std::unique_ptr<FrameBuffer>& Renderer::GetFrameBuffer()const
	{
		return m_frameBuffer;
	}

	const std::shared_ptr<Direct3D>& Renderer::GetD3D() const
	{
		return m_direct3d;
	}
}