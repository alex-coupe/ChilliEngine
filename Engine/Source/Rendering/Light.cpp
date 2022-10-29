#include "Light.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	Light::Light(LightType lightType, Entity& ent)
		:m_lightType(lightType), m_entityId(ent.Uuid)
	{ 
		auto lightComp = std::static_pointer_cast<LightComponent>(ent.GetComponentByType(ComponentType::Light));
		auto transform = ent.GetTransformComponent();

		switch (m_lightType)
		{
		case LightType::DirectionalLight:
			dirLightData.direction = transform->Rotation();
			dirLightData.ambient = lightComp->Ambient();
			dirLightData.diffuse = lightComp->Diffuse();
			dirLightData.specular = lightComp->Specular();
			break;
		case LightType::PointLight:
			pointLightData.position = transform->Translation();
			pointLightData.ambient = lightComp->Ambient();
			pointLightData.diffuse = lightComp->Diffuse();
			pointLightData.specular = lightComp->Specular();
			pointLightData.linear = lightComp->Linear();
			pointLightData.constant = lightComp->Constant();
			pointLightData.quadratic = lightComp->Quadratic();
			break;
		case LightType::Spotlight:
			spotlightData.position = transform->Translation();
			spotlightData.direction = transform->Rotation();
			spotlightData.ambient = lightComp->Ambient();
			spotlightData.diffuse = lightComp->Diffuse();
			spotlightData.specular = lightComp->Specular();
			spotlightData.linear = lightComp->Linear();
			spotlightData.constant = lightComp->Constant();
			spotlightData.quadratic = lightComp->Quadratic();
			spotlightData.cutOff = cos(DirectX::XMConvertToRadians(lightComp->InnerCutOff()));
			spotlightData.outerCutOff = cos(DirectX::XMConvertToRadians(lightComp->OuterCutOff()));
			break;
		}
	}

	void Light::Update()
	{
		auto entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(m_entityId);
		auto lightComp = std::static_pointer_cast<LightComponent>(entity->GetComponentByType(ComponentType::Light));
		auto transform = entity->GetTransformComponent();
		m_lightType = lightComp->GetLightType();
		switch (m_lightType)
		{
		case LightType::DirectionalLight:
			dirLightData.direction = transform->Rotation();
			dirLightData.ambient = lightComp->Ambient();
			dirLightData.diffuse = lightComp->Diffuse();
			dirLightData.specular = lightComp->Specular();
			break;
		case LightType::PointLight:
			pointLightData.position = transform->Translation();
			pointLightData.ambient = lightComp->Ambient();
			pointLightData.diffuse = lightComp->Diffuse();
			pointLightData.specular = lightComp->Specular();
			pointLightData.linear = lightComp->Linear();
			pointLightData.constant = lightComp->Constant();
			pointLightData.quadratic = lightComp->Quadratic();
			break;
		case LightType::Spotlight:
			spotlightData.position = transform->Translation();
			spotlightData.direction = transform->Rotation();
			spotlightData.ambient = lightComp->Ambient();
			spotlightData.diffuse = lightComp->Diffuse();
			spotlightData.specular = lightComp->Specular();
			spotlightData.linear = lightComp->Linear();
			spotlightData.constant = lightComp->Constant();
			spotlightData.quadratic = lightComp->Quadratic();
			spotlightData.cutOff = cos(DirectX::XMConvertToRadians(lightComp->InnerCutOff()));
			spotlightData.outerCutOff = cos(DirectX::XMConvertToRadians(lightComp->OuterCutOff()));
			break;
		}
	}

	LightType Light::GetLightType()
	{
		return m_lightType;
	}
}