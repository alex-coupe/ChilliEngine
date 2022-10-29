#include "PointLight.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	
	PointLight::PointLight(Entity& entity)
		:Light(LightType::PointLight, entity.Uuid)
	{
		auto lightComp = std::static_pointer_cast<LightComponent>(entity.GetComponentByType(ComponentType::Light));
		data.position = entity.GetTransformComponent()->Translation();
		data.ambient = lightComp->Ambient();
		data.diffuse = lightComp->Diffuse();
		data.specular = lightComp->Specular();
		data.linear = lightComp->Linear();
		data.constant = lightComp->Constant();
		data.quadratic = lightComp->Quadratic();
	}

	void PointLight::Update()
	{
		auto entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(m_entityId);
		auto lightComp = std::static_pointer_cast<LightComponent>(entity->GetComponentByType(ComponentType::Light));
		data.position = entity->GetTransformComponent()->Translation();
		data.ambient = lightComp->Ambient();
		data.diffuse = lightComp->Diffuse();
		data.specular = lightComp->Specular();
		data.linear = lightComp->Linear();
		data.constant = lightComp->Constant();
		data.quadratic = lightComp->Quadratic();
	}
	
}