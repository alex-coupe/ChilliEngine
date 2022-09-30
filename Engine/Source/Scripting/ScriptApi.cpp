#include "ScriptApi.h"
#include "mono/jit/jit.h"
#include "../Core/DependencyResolver.h"
#include "../Core/Events.h"
#include "../Core/Timer.h"
#include "../Core/UUID.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {

	static std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<Entity>)>> s_EntityHasComponentFuncs;
#pragma region Core

	static void Log(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		std::string message(str);
		mono_free(str);
		CHILLI_INFO(message);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		const auto& entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(entityID);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

#pragma endregion

#pragma region Transform



#pragma endregion

#pragma region Input

	static bool Input_IsKeyDown(Key keycode)
	{
		return DependencyResolver().ResolveDependency<Events>()->GetKeyPressed(keycode);
	}

#pragma endregion
	template <typename T>
	void ScriptApi::RegisterComponent(ComponentTypes type)
	{
		std::string_view typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');
		std::string_view structName = typeName.substr(pos + 1);
		std::string managedTypename = fmt::format("Chilli.{}", structName);

		MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), 
			DependencyResolver::ResolveDependency<ProjectManager>()->GetCoreScriptAssemblyImage());
		if (!managedType)
		{
			CHILLI_WARN("Could not find component type {}", managedTypename);
			return;
		}
		s_EntityHasComponentFuncs[managedType] = [type](std::shared_ptr<Entity> entity) { return entity->HasComponent(type); };
	}
	void ScriptApi::RegisterComponents()
	{
		RegisterComponent<TransformComponent>(ComponentTypes::Transform);
		RegisterComponent<MeshComponent>(ComponentTypes::Mesh);
		RegisterComponent<RigidBody2DComponent>(ComponentTypes::RigidBody2D);
		RegisterComponent<BoxCollider2DComponent>(ComponentTypes::BoxCollider2D);
		RegisterComponent<CircleColliderComponent>(ComponentTypes::CircleCollider);
		RegisterComponent<ScriptComponent>(ComponentTypes::Script);
	}
	void ScriptApi::Init()
	{
		RegisterComponents();
		mono_add_internal_call("Chilli.InternalCalls::Log", Log);
		mono_add_internal_call("Chilli.InternalCalls::Input_IsKeyDown", Input_IsKeyDown);
		mono_add_internal_call("Chilli.InternalCalls::Entity_HasComponent", Entity_HasComponent);
	}
}