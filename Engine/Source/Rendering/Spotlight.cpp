#include "Spotlight.h"
#include "../ResourceSystem/ProjectManager.h"
namespace Chilli {
	Spotlight::Spotlight(Entity& entity)
		:Light(LightType::Spotlight, entity.Uuid)
	{
		auto lightComp = std::static_pointer_cast<LightComponent>(entity.GetComponentByType(ComponentType::Light));
		auto transform = entity.GetTransformComponent();
		data.position = transform->Translation();
		data.direction = transform->Rotation();
		data.ambient = lightComp->Ambient();
		data.diffuse = lightComp->Diffuse();
		data.specular = lightComp->Specular();
		data.linear = lightComp->Linear();
		data.constant = lightComp->Constant();
		data.quadratic = lightComp->Quadratic();
		data.cutOff = cos(DirectX::XMConvertToRadians(lightComp->InnerCutOff()));
		data.outerCutOff = cos(DirectX::XMConvertToRadians(lightComp->OuterCutOff()));
	}

	void Spotlight::Update()
	{
		auto entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(m_entityId);
		auto lightComp = std::static_pointer_cast<LightComponent>(entity->GetComponentByType(ComponentType::Light));
		auto transform = entity->GetTransformComponent();
		data.position = transform->Translation();
		data.direction = transform->Rotation();
		data.ambient = lightComp->Ambient();
		data.diffuse = lightComp->Diffuse();
		data.specular = lightComp->Specular();
		data.linear = lightComp->Linear();
		data.constant = lightComp->Constant();
		data.quadratic = lightComp->Quadratic();
		data.cutOff = cos(DirectX::XMConvertToRadians(lightComp->InnerCutOff()));
		data.outerCutOff = cos(DirectX::XMConvertToRadians(lightComp->OuterCutOff()));
	}

}