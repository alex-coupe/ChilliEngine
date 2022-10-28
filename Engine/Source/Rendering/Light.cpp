#include "Light.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	using namespace DirectX;

	Light::Light(LightType type, Entity& entity)
		:m_type(type), m_entityId(entity.Uuid)
	{
		auto lightComp = std::static_pointer_cast<LightComponent>(entity.GetComponentByType(ComponentType::Light));
		properties = {};
		properties.position = entity.GetTransformComponent()->Translation();
		properties.direction = entity.GetTransformComponent()->Rotation();
		properties.ambient = lightComp->Ambient();
		properties.diffuse = lightComp->Diffuse();
		properties.specular = lightComp->Specular();
		properties.linear = lightComp->Linear();
		properties.constant = lightComp->Constant();
		properties.quadratic = lightComp->Quadratic();
		properties.cutOff = cos(DirectX::XMConvertToRadians(lightComp->InnerCutOff()));
		properties.outerCutOff = cos(DirectX::XMConvertToRadians(lightComp->OuterCutOff()));
	}

	void Light::Update()
	{
		auto entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(m_entityId);
		auto lightComp = std::static_pointer_cast<LightComponent>(entity->GetComponentByType(ComponentType::Light));
		properties.position = entity->GetTransformComponent()->Translation();
		properties.direction = entity->GetTransformComponent()->Rotation();
		properties.ambient = lightComp->Ambient();
		properties.diffuse = lightComp->Diffuse();
		properties.specular = lightComp->Specular();
		properties.linear = lightComp->Linear();
		properties.constant = lightComp->Constant();
		properties.quadratic = lightComp->Quadratic();
		properties.cutOff = cos(DirectX::XMConvertToRadians(lightComp->InnerCutOff()));
		properties.outerCutOff = cos(DirectX::XMConvertToRadians(lightComp->OuterCutOff()));
	}
	
}