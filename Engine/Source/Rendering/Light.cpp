#include "Light.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	using namespace DirectX;

	Light::Light(LightType type, XMFLOAT3 pos, XMFLOAT3 dir, DirectX::XMFLOAT3 col, UUID entId)
		:m_type(type), m_color(col), m_position(pos), m_direction(dir), m_entityId(entId)
	{
		
	}

	XMFLOAT3& Light::GetPosition()
	{
		m_position = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(m_entityId)->GetTransformComponent()->Translation();
		return m_position;
	}

	XMFLOAT3& Light::GetColor()
	{
		m_color = std::static_pointer_cast<LightComponent>(DependencyResolver::ResolveDependency<ProjectManager>()
			->GetCurrentScene()
			->GetEntityByUUID(m_entityId)
			->GetComponentByType(ComponentType::Light))
			->Color();
		return m_color;
	}
	
}