#include "DirectionalLight.h"
#include "../ResourceSystem/ProjectManager.h"
namespace Chilli {

	DirectionalLight::DirectionalLight(Entity& entity)
		:Light(LightType::DirectionalLight, entity.Uuid)
	{
		auto lightComp = std::static_pointer_cast<LightComponent>(entity.GetComponentByType(ComponentType::Light));
		data.direction = entity.GetTransformComponent()->Rotation();
		data.ambient = lightComp->Ambient();
		data.diffuse = lightComp->Diffuse();
		data.specular = lightComp->Specular();
	}

	void DirectionalLight::Update()
	{
		auto entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(m_entityId);
		auto lightComp = std::static_pointer_cast<LightComponent>(entity->GetComponentByType(ComponentType::Light));
		data.direction = entity->GetTransformComponent()->Rotation();
		data.ambient = lightComp->Ambient();
		data.diffuse = lightComp->Diffuse();
		data.specular = lightComp->Specular();
	}
}