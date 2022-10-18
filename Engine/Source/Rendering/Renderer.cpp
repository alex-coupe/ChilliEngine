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
		m_editorCamera = std::make_unique<Camera>(1.0f, m_aspectRatio, 0.5f, 100.0f,CameraType::Editor,ProjectionType::Perspective);
		m_frameBuffer = std::make_unique<FrameBuffer>(width, height, m_direct3d);
		m_renderCamera = m_editorCamera.get();
	}

	Renderer::~Renderer()
	{
		m_frameBuffer.reset();
		m_editorCamera.reset();
		m_renderJobs.clear();
		m_direct3d.reset();
	}

	const std::unique_ptr<Camera>& Renderer::GetEditorCamera()
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
		const auto& entities = m_sceneManager->GetCurrentScene()->GetEntities();
		if (m_renderJobs.size() != entities.size())
		{
			m_renderJobs.clear();
			for (const auto& entity : entities)
			{
				if (entity->HasComponent(ComponentType::Mesh)
					&& entity->HasComponent(ComponentType::Transform))
				{
					m_renderJobs.emplace_back(RenderJob(m_direct3d,entity));
				}
			}
		}
		m_frameBuffer->Bind();
		for (auto& job : m_renderJobs)
		{
			job.Update(m_renderCamera);
			job.Draw();
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