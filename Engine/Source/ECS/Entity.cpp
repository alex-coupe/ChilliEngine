#include "Entity.h"

Engine::ECS::Entity::Entity(const std::string& name)
	: m_name(name), m_uuid()
{
	m_components.emplace_back(ComponentFactory::MakeTransformComponent());
}

Engine::ECS::Entity::Entity(const std::string& name, Engine::Utilities::UUID uuid, const rapidjson::Value& components)
	: m_name(name), m_uuid(uuid)
{
	for (unsigned int i = 0; i < components.Size(); i++)
	{
		switch (components[i]["Type"].GetInt())
		{
			case (int)ComponentTypes::Transform:
			{
				DirectX::XMFLOAT3 translation = { components[i]["PosX"].GetFloat(),components[i]["PosY"].GetFloat(), components[i]["PosZ"].GetFloat() };
				DirectX::XMFLOAT3 rotation = { components[i]["RotX"].GetFloat(),components[i]["RotY"].GetFloat(), components[i]["RotZ"].GetFloat() };
				DirectX::XMFLOAT3 scale = { components[i]["ScaleX"].GetFloat(),components[i]["ScaleY"].GetFloat(), components[i]["ScaleZ"].GetFloat() };
				m_components.emplace_back(std::make_shared<TransformComponent>(translation, rotation, scale));
				break;
			}
			case (int)ComponentTypes::Mesh:
				m_components.emplace_back(std::make_shared<MeshComponent>(components[i]["MeshUuid"].GetString()));
				break;
			case (int)ComponentTypes::RigidBody2D:
				m_components.emplace_back(std::make_shared<RigidBody2DComponent>((BodyType)components[i]["BodyType"].GetInt(), (bool)components[i]["FixedRotation"].GetInt()));
				break;
			case (int)ComponentTypes::BoxCollider2D:
			{
				DirectX::XMFLOAT2 size = { components[i]["SizeX"].GetFloat(),components[i]["SizeY"].GetFloat()};
				DirectX::XMFLOAT2 offset = { components[i]["OffSetX"].GetFloat(),components[i]["OffSetY"].GetFloat()};
				m_components.emplace_back(std::make_shared<BoxCollider2DComponent>(size,offset, 
					components[i]["Density"].GetFloat(), components[i]["Friction"].GetFloat(),
					components[i]["Restitution"].GetFloat(),components[i]["RestitutionThreshold"].GetFloat()));
				break;
			}
			case (int)ComponentTypes::CircleCollider:
			{
				DirectX::XMFLOAT2 offset = { components[i]["OffSetX"].GetFloat(),components[i]["OffSetY"].GetFloat() };
				m_components.emplace_back(std::make_shared<CircleColliderComponent>(components[i]["Radius"].GetFloat(), offset,
					components[i]["Density"].GetFloat(), components[i]["Friction"].GetFloat(),
					components[i]["Restitution"].GetFloat(), components[i]["RestitutionThreshold"].GetFloat()));
			}
				break;
		}
	}
}

const std::string& Engine::ECS::Entity::GetName()const
{
	return m_name;
}

const std::shared_ptr<Engine::ECS::TransformComponent> Engine::ECS::Entity::GetTransformComponent()
{
	for (const auto& comp : m_components)
	{
		if (comp->GetComponentType() == ComponentTypes::Transform)
			return std::static_pointer_cast<TransformComponent>(comp);
	}
	return nullptr;
}

void Engine::ECS::Entity::InitPhysics(std::unique_ptr<b2World>& physicsWorld)
{
	auto& transform = GetTransformComponent();
	if (HasComponent(ComponentTypes::RigidBody2D))
	{
		auto rigidBody = CreateRigidBody(physicsWorld, transform);
		if (HasComponent(ComponentTypes::BoxCollider2D))
		{
			CreateBoxCollider(rigidBody, transform);
		}

		if (HasComponent(ComponentTypes::CircleCollider))
		{
			CreateCircleCollider(rigidBody, transform);
		}
	}
}

void Engine::ECS::Entity::UpdatePhysics()
{
	auto& transform = GetTransformComponent();
	const auto& rb2d = std::static_pointer_cast<Engine::ECS::RigidBody2DComponent>(GetComponentByType(ComponentTypes::RigidBody2D));

	const auto& position = rb2d->GetBody()->GetPosition();
	transform->GetTranslation().x = position.x;
	transform->GetTranslation().y = position.y;
	transform->GetRotation().z = rb2d->GetBody()->GetAngle();
}

std::shared_ptr<Engine::ECS::Entity> Engine::ECS::Entity::Clone(Entity& entity)
{
	auto clone = std::make_shared<Entity>(entity.GetName());
	clone->RemoveComponent(ComponentTypes::Transform);

	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	for (const auto& compo : entity.GetComponents())
	{
		clone->AddComponent(compo);
	}

	auto transform = clone->GetTransformComponent();

	translation.x = transform->GetTranslation().x;
	translation.y = transform->GetTranslation().y;
	translation.z = transform->GetTranslation().z;

	rotation.x = transform->GetRotation().x;
	rotation.y = transform->GetRotation().y;
	rotation.z = transform->GetRotation().z;

	scale.x = transform->GetScale().x;
	scale.y = transform->GetScale().y;
	scale.z = transform->GetScale().z;

	clone->RemoveComponent(ComponentTypes::Transform);
	clone->AddComponent(std::make_shared<TransformComponent>(translation, rotation, scale));

	return clone;
}

b2Body* Engine::ECS::Entity::CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform)
{
	const auto& rb2d = std::static_pointer_cast<Engine::ECS::RigidBody2DComponent>(GetComponentByType(ComponentTypes::RigidBody2D));
	b2BodyDef bodyDef;
	bodyDef.position.Set(transform->GetTranslation().x, transform->GetTranslation().y);
	bodyDef.angle = transform->GetRotation().z;
	switch (rb2d->GetBodyType()) {
	case BodyType::Static:
		bodyDef.type = b2_staticBody;
		break;
	case BodyType::Dynamic:
		bodyDef.type = b2_dynamicBody;
		break;
	case BodyType::Kinematic:
		bodyDef.type = b2_kinematicBody;
		break;
	default:
		bodyDef.type = b2_staticBody;
		break;
	}

	auto rigidBody = physicsWorld->CreateBody(&bodyDef);
	rigidBody->SetFixedRotation(*rb2d->GetFixedRotation());
	rb2d->SetRigidBody(rigidBody);
	return rigidBody;
}

void Engine::ECS::Entity::CreateBoxCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform)
{
	const auto& bc2d = std::static_pointer_cast<Engine::ECS::BoxCollider2DComponent>(GetComponentByType(ComponentTypes::BoxCollider2D));
	b2PolygonShape boxShape;
	boxShape.SetAsBox(bc2d->GetSize().x * transform->GetScale().x,
		bc2d->GetSize().y * transform->GetScale().y);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = bc2d->GetDensity();
	fixtureDef.friction = bc2d->GetFriction();
	fixtureDef.restitution = bc2d->GetRestitution();;
	fixtureDef.restitutionThreshold = bc2d->GetRestituitonThreshold();
	rigidBody->CreateFixture(&fixtureDef);
}

void Engine::ECS::Entity::CreateCircleCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform)
{
	const auto& circleCollider = std::static_pointer_cast<Engine::ECS::CircleColliderComponent>
		(GetComponentByType(ComponentTypes::CircleCollider));
	b2CircleShape circleShape;
	circleShape.m_p.Set(circleCollider->GetOffset().x, circleCollider->GetOffset().y);
	circleShape.m_radius = transform->GetScale().x * circleCollider->GetRadius();
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = circleCollider->GetDensity();
	fixtureDef.friction = circleCollider->GetFriction();
	fixtureDef.restitution = circleCollider->GetRestitution();;
	fixtureDef.restitutionThreshold = circleCollider->GetRestituitonThreshold();
	rigidBody->CreateFixture(&fixtureDef);
}

const std::vector<std::shared_ptr<Engine::ECS::Component>>& Engine::ECS::Entity::GetComponents()const
{
	return m_components;
}

std::shared_ptr<Engine::ECS::Component>  Engine::ECS::Entity::GetComponentByType(ComponentTypes type)
{
	for (const auto& comp : m_components)
	{
		if (comp->GetComponentType() == type)
			return comp;
	}
	return nullptr;
}

bool Engine::ECS::Entity::HasComponent(ComponentTypes type)
{
	auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
		return rhs->GetComponentType() == type;
		});
	return contains != m_components.cend();
}

void Engine::ECS::Entity::AddComponent(ComponentTypes type)
{
	if (auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
		return rhs->GetComponentType() == type;
		}); contains == m_components.cend())
	{
		switch (type)
		{
		case ComponentTypes::Mesh:
			m_components.emplace_back(ComponentFactory::MakeMeshComponent());
			break;
		case ComponentTypes::RigidBody2D:
			m_components.emplace_back(ComponentFactory::MakeRigidBody2DComponent());
			break;
		case ComponentTypes::BoxCollider2D:
			m_components.emplace_back(ComponentFactory::MakeBoxCollider2DComponent());
			break;
		case ComponentTypes::CircleCollider:
			m_components.emplace_back(ComponentFactory::MakeCircleColliderComponent());
			break;
		
		}
	}
}

void Engine::ECS::Entity::AddComponent(std::shared_ptr<Component> component)
{
	m_components.push_back(component);
}

void Engine::ECS::Entity::RemoveComponent(ComponentTypes type)
{
	if (auto m_compoIterator = std::find_if(m_components.begin(), m_components.end(), [type](const std::shared_ptr<Component> rhs) 
		{
			return rhs->GetComponentType() == type;
		}); m_compoIterator != m_components.end())
	{
		m_components.erase(m_compoIterator);
	}
}

const std::string Engine::ECS::Entity::Serialize() const
{
	std::stringstream ss;
	ss << "{\"Name\":\"" << m_name << "\", \"Uuid\":\"" << m_uuid.GetUUID() << "\", \"Components\":[";
	for (size_t i = 0; i < m_components.size(); i++)
	{
		ss << m_components[i]->Serialize();
		if (i != m_components.size() - 1)
			ss << ",";
	}
	ss << "]}";
	return ss.str();
}

const Engine::Utilities::UUID& Engine::ECS::Entity::GetUUID() const
{
	return m_uuid;
}

std::shared_ptr<Engine::ECS::Component> Engine::ECS::Entity::GetComponentByName(const char* name)
{
	for (const auto& comp : m_components)
	{
		if (comp->GetName() == name)
			return comp;
	}
	return nullptr;
}






