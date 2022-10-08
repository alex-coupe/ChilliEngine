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
		m_projMatrix = DirectX::XMMatrixPerspectiveLH(1.0f, m_aspectRatio, 0.5f, 100.0f);
		m_transformationCBuff = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(ConstantBufferType::Vertex, m_direct3d);
		m_color = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>(ConstantBufferType::Pixel, m_direct3d);
		m_transformationCBuff->Bind();
		m_color->Bind();
		DirectX::XMFLOAT3 camPosition = { 0.0f,0.0f,-5.0f };
		m_editorCamera = std::make_unique<Camera>(camPosition, (float)width, (float)height);
		m_frameBuffer = std::make_unique<FrameBuffer>(width, height, m_direct3d);
	}

	Renderer::~Renderer()
	{
		m_frameBuffer.release();
		m_drawables.clear();
		m_color.reset();
		m_transformationCBuff.reset();
		m_direct3d->ShutdownD3D();
		m_direct3d.reset();
	}

	const std::unique_ptr<Camera>& Renderer::GetEditorCamera()
	{
		return m_editorCamera;
	}

	const DirectX::XMMATRIX& Renderer::GetProjectionMatrix() const
	{
		return m_projMatrix;
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
		m_frameBuffer->Unbind();
		m_frameBuffer.release();
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
			auto transform = DirectX::XMMatrixTranspose(drawable->GetTransform() * m_editorCamera->GetViewMatrix() * GetProjectionMatrix());
			m_transformationCBuff->Update(transform);
			m_color->Update(drawable->GetColor());
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