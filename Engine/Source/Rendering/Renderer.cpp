#include "Renderer.h"
#include <Windows.h>
#include "../Gui/GuiManager.h"
#include "../Core/Events.h"
#include "../Core/Window.h"
#include "../ECS/MeshComponent.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/LightComponent.h"

namespace Chilli {

	Renderer::Renderer(int64_t width, int64_t height, void* handle)
		: m_aspectRatio((float)height / (float)width)
	{
		m_direct3d = std::make_shared<Direct3D>((HWND)handle, width, height);
	}

	Renderer::~Renderer()
	{
		m_renderJobs.clear();
		m_direct3d.reset();
	}

	void Renderer::ClearRenderJobs()
	{
		m_renderJobs.clear();
	}

	Camera* Renderer::GetActiveCamera()
	{
		return m_renderCamera;
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

		m_aspectRatio = (float)height / (float)width;
		m_direct3d->HandleWindowResize(width, height);
		m_appLayer->OnResize();	
	}

	void Renderer::UpdateLightCount()
	{
		m_lightCount.dirLightCount = 0;
		m_lightCount.pointLightCount = 0;
		m_lightCount.spotLightCount = 0;

		for (const auto& light : m_lights)
		{
			switch (light.second->GetLightType())
			{
			case LightType::DirectionalLight:
				m_lightCount.dirLightCount++;
				break;
			case LightType::PointLight:
				m_lightCount.pointLightCount++;
				break;
			case LightType::Spotlight:
				m_lightCount.spotLightCount++;
				break;
			}
		}
	}

	void Renderer::ProcessRenderJobs()
	{
		auto currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();

		UpdateLightCount();
		for (auto& job : m_renderJobs)
		{
			job.second.Update(m_renderCamera, m_lights, currentScene->GetSceneState(), m_lightCount);
			job.second.Draw(currentScene->GetSceneState());
		}
	}

	void Renderer::ProcessFrame()
	{
		m_appLayer->OnRender();
		ProcessRenderJobs();
		m_direct3d->SetBackBufferRenderTarget();
		m_direct3d->ClearBackBuffer();
		GuiManager::BeginFrame();
		m_appLayer->OnRenderGui();
		GuiManager::EndFrame();
		m_direct3d->EndFrame();
	}

	uint64_t Renderer::AddRenderJob(Entity& entity, RenderJobType type)
	{
		uint64_t id = UUID().Get();
		m_renderJobs.emplace(id, std::move(RenderJob(m_direct3d, entity, type)));
		return id;
	}

	void Renderer::RemoveRenderJob(uint64_t jobId)
	{
		auto job = m_renderJobs.find(jobId);
		if (job->first)
			m_renderJobs.erase(jobId);
	}

	void Renderer::SetRenderCamera(Camera* cam)
	{
		m_renderCamera = cam;
	}

	void Renderer::CreateLight(Entity& lightEnt)
	{
		if (lightEnt.HasComponent(ComponentType::Light))
		{
			auto lightComp = std::static_pointer_cast<LightComponent>(lightEnt.GetComponentByType(ComponentType::Light));
			auto lightType = lightComp->GetLightType();
			m_lights.emplace(lightEnt.Uuid.Get(), std::make_unique<Light>(lightType, lightEnt));
			switch (lightType)
			{
			case LightType::DirectionalLight:
				m_lightCount.dirLightCount++;
				break;
			case LightType::PointLight:
				m_lightCount.pointLightCount++;
				break;
			case LightType::Spotlight:
				m_lightCount.spotLightCount++;
				break;
			}
		}
	}
	void Renderer::DestroyLight(UUID entId)
	{
		auto lightsItr = m_lights.find(entId.Get());

		if (lightsItr != m_lights.end())
			m_lights.erase(lightsItr);
	}

	const float Renderer::GetAspectRatio()const
	{
		return m_aspectRatio;
	}

	const std::shared_ptr<Direct3D>& Renderer::GetD3D() const
	{
		return m_direct3d;
	}

	void Renderer::SetAppLayer(const std::shared_ptr<Layer>& layer)
	{
		m_appLayer = layer;
	}
	float Renderer::GetDisplayWindowAspectRatio() const
	{
		return m_appLayer->GetDisplayAspectRatio();
	}
}