#include "Entity.h"

namespace Chilli {

	Entity::Entity(const std::string& name)
		: m_name(name), Uuid()
	{
		m_components.emplace_back(ComponentFactory::MakeTransformComponent());
	}

	Entity::Entity(const std::string& name, UUID uuid, const rapidjson::Value& components)
		: m_name(name), Uuid(uuid)
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
				m_components.emplace_back(std::make_shared<MeshComponent>(components[i]["MeshUuid"].GetInt()));
				break;
			case (int)ComponentTypes::RigidBody2D:
				m_components.emplace_back(std::make_shared<RigidBody2DComponent>((BodyType)components[i]["BodyType"].GetInt(), (bool)components[i]["FixedRotation"].GetInt()));
				break;
			case (int)ComponentTypes::BoxCollider2D:
			{
				DirectX::XMFLOAT2 size = { components[i]["SizeX"].GetFloat(),components[i]["SizeY"].GetFloat() };
				DirectX::XMFLOAT2 offset = { components[i]["OffSetX"].GetFloat(),components[i]["OffSetY"].GetFloat() };
				m_components.emplace_back(std::make_shared<BoxCollider2DComponent>(size, offset,
					components[i]["Density"].GetFloat(), components[i]["Friction"].GetFloat(),
					components[i]["Restitution"].GetFloat(), components[i]["RestitutionThreshold"].GetFloat()));
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
			case (int)ComponentTypes::Script:
				m_components.emplace_back(std::make_shared<ScriptComponent>(components[i]["ScriptClassName"].GetString()));
				ScriptInstanceRepository::MakeScriptInstance(components[i]["ScriptClassName"].GetString(),Uuid.Get());
				break;
			}
		}
	}

	const std::string& Entity::GetName()const
	{
		return m_name;
	}

	const std::shared_ptr<TransformComponent> Entity::GetTransformComponent()
	{
		for (const auto& comp : m_components)
		{
			if (comp->GetComponentType() == ComponentTypes::Transform)
				return std::static_pointer_cast<TransformComponent>(comp);
		}
		return nullptr;
	}

	void Entity::InitPhysics(std::unique_ptr<b2World>& physicsWorld)
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

	void Entity::UpdatePhysics()
	{
		auto& transform = GetTransformComponent();
		const auto& rb2d = std::static_pointer_cast<RigidBody2DComponent>(GetComponentByType(ComponentTypes::RigidBody2D));
		if (rb2d)
		{
			const auto& position = rb2d->GetBody()->GetPosition();
			transform->Translation().x = position.x;
			transform->Translation().y = position.y;
			transform->Rotation().z = rb2d->GetBody()->GetAngle();
		}
	}

	std::shared_ptr<Entity> Entity::Clone(Entity& entity)
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

		translation.x = transform->Translation().x;
		translation.y = transform->Translation().y;
		translation.z = transform->Translation().z;

		rotation.x = transform->Rotation().x;
		rotation.y = transform->Rotation().y;
		rotation.z = transform->Rotation().z;

		scale.x = transform->Scale().x;
		scale.y = transform->Scale().y;
		scale.z = transform->Scale().z;

		clone->RemoveComponent(ComponentTypes::Transform);
		clone->AddComponent(std::make_shared<TransformComponent>(translation, rotation, scale));

		return clone;
	}

	b2Body* Entity::CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform)
	{
		const auto& rb2d = std::static_pointer_cast<RigidBody2DComponent>(GetComponentByType(ComponentTypes::RigidBody2D));
		b2BodyDef bodyDef;
		bodyDef.position.Set(transform->Translation().x, transform->Translation().y);
		bodyDef.angle = transform->Rotation().z;
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

	void Entity::CreateBoxCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform)
	{
		const auto& bc2d = std::static_pointer_cast<BoxCollider2DComponent>(GetComponentByType(ComponentTypes::BoxCollider2D));
		b2PolygonShape boxShape;
		boxShape.SetAsBox(bc2d->GetSize().x * transform->Scale().x,
			bc2d->GetSize().y * transform->Scale().y);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = bc2d->GetDensity();
		fixtureDef.friction = bc2d->GetFriction();
		fixtureDef.restitution = bc2d->GetRestitution();;
		fixtureDef.restitutionThreshold = bc2d->GetRestituitonThreshold();
		rigidBody->CreateFixture(&fixtureDef);
	}

	void Entity::CreateCircleCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform)
	{
		const auto& circleCollider = std::static_pointer_cast<CircleColliderComponent>(GetComponentByType(ComponentTypes::CircleCollider));
		b2CircleShape circleShape;
		circleShape.m_p.Set(circleCollider->GetOffset().x, circleCollider->GetOffset().y);
		circleShape.m_radius = transform->Scale().x * circleCollider->GetRadius();
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = circleCollider->GetDensity();
		fixtureDef.friction = circleCollider->GetFriction();
		fixtureDef.restitution = circleCollider->GetRestitution();;
		fixtureDef.restitutionThreshold = circleCollider->GetRestituitonThreshold();
		rigidBody->CreateFixture(&fixtureDef);
	}

	const std::vector<std::shared_ptr<Component>>& Entity::GetComponents()const
	{
		return m_components;
	}

	std::shared_ptr<Component>  Entity::GetComponentByType(ComponentTypes type)
	{
		for (const auto& comp : m_components)
		{
			if (comp->GetComponentType() == type)
				return comp;
		}
		return nullptr;
	}

	bool Entity::HasComponent(ComponentTypes type)
	{
		auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
			return rhs->GetComponentType() == type;
			});
		return contains != m_components.cend();
	}

	void Entity::AddComponent(ComponentTypes type)
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
			case ComponentTypes::Script:
				m_components.emplace_back(ComponentFactory::MakeScriptComponent());
				break;
			}
		}
	}

	void Entity::AddComponent(std::shared_ptr<Component> component)
	{
		m_components.push_back(component);
	}

	void Entity::RemoveComponent(ComponentTypes type)
	{
		if (auto m_compoIterator = std::find_if(m_components.begin(), m_components.end(), [type](const std::shared_ptr<Component> rhs)
			{
				return rhs->GetComponentType() == type;
			}); m_compoIterator != m_components.end())
		{
			m_components.erase(m_compoIterator);
			if (type == ComponentTypes::Script)
				ScriptInstanceRepository::RemoveScriptInstance(Uuid.Get());
		}
	}

	const std::string Entity::Serialize() const
	{
		std::stringstream ss;
		ss << "{\"Name\":\"" << m_name << "\", \"Uuid\":" << Uuid.Get() << ", \"Components\":[";
		for (size_t i = 0; i < m_components.size(); i++)
		{
			ss << m_components[i]->Serialize();
			if (i != m_components.size() - 1)
				ss << ",";
		}
		ss << "]}";
		return ss.str();
	}

	std::shared_ptr<Component> Entity::GetComponentByName(const char* name)
	{
		for (const auto& comp : m_components)
		{
			if (comp->GetName() == name)
				return comp;
		}
		return nullptr;
	}
}





