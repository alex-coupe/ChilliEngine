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
			case (int)ComponentType::Transform:
			{
				DirectX::XMFLOAT3 translation = { components[i]["PosX"].GetFloat(),components[i]["PosY"].GetFloat(), components[i]["PosZ"].GetFloat() };
				DirectX::XMFLOAT3 rotation = { components[i]["RotX"].GetFloat(),components[i]["RotY"].GetFloat(), components[i]["RotZ"].GetFloat() };
				DirectX::XMFLOAT3 scale = { components[i]["ScaleX"].GetFloat(),components[i]["ScaleY"].GetFloat(), components[i]["ScaleZ"].GetFloat() };
				m_components.emplace_back(std::make_shared<TransformComponent>(translation, rotation, scale));
				break;
			}
			case (int)ComponentType::Mesh:
				m_components.emplace_back(std::make_shared<MeshComponent>(components[i]["MeshUuid"].GetInt()));
				break;
			case (int)ComponentType::RigidBody2D:
				m_components.emplace_back(std::make_shared<RigidBody2DComponent>((BodyType)components[i]["BodyType"].GetInt(), (bool)components[i]["FixedRotation"].GetInt()));
				break;
			case (int)ComponentType::BoxCollider2D:
			{
				DirectX::XMFLOAT2 size = { components[i]["SizeX"].GetFloat(),components[i]["SizeY"].GetFloat() };
				DirectX::XMFLOAT2 offset = { components[i]["OffSetX"].GetFloat(),components[i]["OffSetY"].GetFloat() };
				m_components.emplace_back(std::make_shared<BoxCollider2DComponent>(size, offset,
					components[i]["Density"].GetFloat(), components[i]["Friction"].GetFloat(),
					components[i]["Restitution"].GetFloat(), components[i]["RestitutionThreshold"].GetFloat()));
				break;
			}
			case (int)ComponentType::CircleCollider:
			{
				DirectX::XMFLOAT2 offset = { components[i]["OffSetX"].GetFloat(),components[i]["OffSetY"].GetFloat() };
				m_components.emplace_back(std::make_shared<CircleColliderComponent>(components[i]["Radius"].GetFloat(), offset,
					components[i]["Density"].GetFloat(), components[i]["Friction"].GetFloat(),
					components[i]["Restitution"].GetFloat(), components[i]["RestitutionThreshold"].GetFloat()));
			}
			break;
			case (int)ComponentType::Script:
				m_components.emplace_back(std::make_shared<ScriptComponent>(components[i]["ScriptClassName"].GetString()));
				auto scriptInstance = ScriptInstanceRepository::MakeScriptInstance(components[i]["ScriptClassName"].GetString(),Uuid.Get());
				if (components[i].HasMember("Fields"))
				{
					auto scriptFields = components[i]["Fields"].GetArray();
					for (unsigned int i = 0; i < scriptFields.Size(); i++)
					{
						auto name = scriptFields[i]["Name"].GetString();
						auto type = (FieldType)scriptFields[i]["Type"].GetInt();
						if (scriptInstance->HasField(name))
						{
							switch (type)
							{
							case FieldType::Float:
								scriptInstance->SetFieldValue<float>(name, scriptFields[i]["Value"].GetFloat());
								break;
							case FieldType::Bool:
								scriptInstance->SetFieldValue<bool>(name, scriptFields[i]["Value"].GetBool());
								break;
							case FieldType::Byte:
								scriptInstance->SetFieldValue<uint8_t>(name, scriptFields[i]["Value"].GetUint());
								break;
							case FieldType::Char:
								scriptInstance->SetFieldValue<char>(name, scriptFields[i]["Value"].GetInt());
								break;
							case FieldType::Double:
								scriptInstance->SetFieldValue<double>(name, scriptFields[i]["Value"].GetDouble());
								break;
							case FieldType::Entity:
							case FieldType::ULong:
								scriptInstance->SetFieldValue<uint64_t>(name, scriptFields[i]["Value"].GetUint64());
								break;
							case FieldType::Int:
								scriptInstance->SetFieldValue<int32_t>(name, scriptFields[i]["Value"].GetInt());
								break;
							case FieldType::Long:
								scriptInstance->SetFieldValue<int64_t>(name, scriptFields[i]["Value"].GetInt64());
								break;
							case FieldType::Short:
								scriptInstance->SetFieldValue<int16_t>(name, scriptFields[i]["Value"].GetInt());
								break;
							case FieldType::UInt:
								scriptInstance->SetFieldValue<uint32_t>(name, scriptFields[i]["Value"].GetUint());
								break;
							case FieldType::UShort:
								scriptInstance->SetFieldValue<uint16_t>(name, scriptFields[i]["Value"].GetUint());
								break;
							case FieldType::Vector2:
							{
								auto values = scriptFields[i]["Value"].GetArray();
								DirectX::XMFLOAT2 value = { values[0]["X"].GetFloat(),values[0]["Y"].GetFloat() };
								scriptInstance->SetFieldValue<DirectX::XMFLOAT2>(name, value);
							}
							break;
							case FieldType::Vector3:
							{
								auto values = scriptFields[i]["Value"].GetArray();
								DirectX::XMFLOAT3 value = { values[0]["X"].GetFloat(),values[0]["Y"].GetFloat(), values[0]["Z"].GetFloat() };
								scriptInstance->SetFieldValue<DirectX::XMFLOAT3>(name, value);
							}
							break;
							case FieldType::Vector4:
							{
								auto values = scriptFields[i]["Value"].GetArray();
								DirectX::XMFLOAT4 value = { values[0]["X"].GetFloat(),values[0]["Y"].GetFloat(), values[0]["Z"].GetFloat(), values[0]["W"].GetFloat() };
								scriptInstance->SetFieldValue<DirectX::XMFLOAT4>(name, value);
							}
							break;
							default:
								break;
							}
						}
					}
				}
				break;
			}
		}
	}

	Entity::Entity(const Entity& rhs)
		: m_name(rhs.m_name)
	{
		for (const auto& component : rhs.m_components)
		{
			switch (component->GetComponentType())
			{
			case ComponentType::Transform:
			{
				auto pointer = std::static_pointer_cast<TransformComponent>(component);
				m_components.emplace_back(std::make_shared<TransformComponent>(*pointer));
			}
				break;
			case ComponentType::Mesh:
			{
				auto pointer = std::static_pointer_cast<MeshComponent>(component);
				m_components.emplace_back(std::make_shared<MeshComponent>(*pointer));
			}
				break;
			case ComponentType::Script:
			{
				auto pointer = std::static_pointer_cast<ScriptComponent>(component);
				m_components.emplace_back(std::make_shared<ScriptComponent>(*pointer));
			}
				break;
			case ComponentType::RigidBody2D:
			{
				auto pointer = std::static_pointer_cast<RigidBody2DComponent>(component);
				m_components.emplace_back(std::make_shared<RigidBody2DComponent>(*pointer));
			}
			break;
			case ComponentType::BoxCollider2D:
			{
				auto pointer = std::static_pointer_cast<BoxCollider2DComponent>(component);
				m_components.emplace_back(std::make_shared<BoxCollider2DComponent>(*pointer));
			}
			break;
			case ComponentType::CircleCollider:
			{
				auto pointer = std::static_pointer_cast<CircleColliderComponent>(component);
				m_components.emplace_back(std::make_shared<CircleColliderComponent>(*pointer));
			}
			break;
			}
		}
	}

	void Entity::Clone(const Entity& rhs)
	{
		m_name = rhs.m_name;
		for (int i = 0; i < m_components.size(); i++)
		{
			m_components[i]->Clone(rhs.m_components[i]);
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
			if (comp->GetComponentType() == ComponentType::Transform)
				return std::static_pointer_cast<TransformComponent>(comp);
		}
		return nullptr;
	}

	void Entity::InitPhysics(std::unique_ptr<b2World>& physicsWorld)
	{
		auto& transform = GetTransformComponent();
		if (HasComponent(ComponentType::RigidBody2D))
		{
			auto rigidBody = CreateRigidBody(physicsWorld, transform);
			if (HasComponent(ComponentType::BoxCollider2D))
			{
				CreateBoxCollider(rigidBody, transform);
			}

			if (HasComponent(ComponentType::CircleCollider))
			{
				CreateCircleCollider(rigidBody, transform);
			}
		}
	}

	void Entity::UpdatePhysics()
	{
		auto& transform = GetTransformComponent();
		const auto& rb2d = std::static_pointer_cast<RigidBody2DComponent>(GetComponentByType(ComponentType::RigidBody2D));
		if (rb2d)
		{
			const auto& position = rb2d->GetBody()->GetPosition();
			transform->Translation().x = position.x;
			transform->Translation().y = position.y;
			transform->Rotation().z = rb2d->GetBody()->GetAngle();
		}
	}

	b2Body* Entity::CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform)
	{
		const auto& rb2d = std::static_pointer_cast<RigidBody2DComponent>(GetComponentByType(ComponentType::RigidBody2D));
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
		const auto& bc2d = std::static_pointer_cast<BoxCollider2DComponent>(GetComponentByType(ComponentType::BoxCollider2D));
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
		const auto& circleCollider = std::static_pointer_cast<CircleColliderComponent>(GetComponentByType(ComponentType::CircleCollider));
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

	std::shared_ptr<Component>  Entity::GetComponentByType(ComponentType type)
	{
		for (const auto& comp : m_components)
		{
			if (comp->GetComponentType() == type)
				return comp;
		}
		return nullptr;
	}

	bool Entity::HasComponent(ComponentType type)
	{
		auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
			return rhs->GetComponentType() == type;
			});
		return contains != m_components.cend();
	}

	void Entity::AddComponent(ComponentType type)
	{
		if (auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
			return rhs->GetComponentType() == type;
			}); contains == m_components.cend())
		{
			switch (type)
			{
			case ComponentType::Mesh:
				m_components.emplace_back(ComponentFactory::MakeMeshComponent());
				break;
			case ComponentType::RigidBody2D:
				m_components.emplace_back(ComponentFactory::MakeRigidBody2DComponent());
				break;
			case ComponentType::BoxCollider2D:
				m_components.emplace_back(ComponentFactory::MakeBoxCollider2DComponent());
				break;
			case ComponentType::CircleCollider:
				m_components.emplace_back(ComponentFactory::MakeCircleColliderComponent());
				break;
			case ComponentType::Script:
				m_components.emplace_back(ComponentFactory::MakeScriptComponent());
				break;
			}
		}
	}

	void Entity::AddComponent(std::shared_ptr<Component> component)
	{
		m_components.push_back(component);
	}

	void Entity::RemoveComponent(ComponentType type)
	{
		if (auto m_compoIterator = std::find_if(m_components.begin(), m_components.end(), [type](const std::shared_ptr<Component> rhs)
			{
				return rhs->GetComponentType() == type;
			}); m_compoIterator != m_components.end())
		{
			m_components.erase(m_compoIterator);
			if (type == ComponentType::Script)
				ScriptInstanceRepository::RemoveScriptInstance(Uuid.Get());
		}
	}

	const std::string Entity::Serialize() const
	{
		std::stringstream ss;
		ss << "{\"Name\":\"" << m_name << "\", \"Uuid\":" << Uuid.Get() << ", \"Components\":[";
		for (size_t i = 0; i < m_components.size(); i++)
		{
			ss << m_components[i]->Serialize(Uuid.Get());
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





