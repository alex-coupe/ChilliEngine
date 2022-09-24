#include "ScriptApi.h"
#include "mono/jit/jit.h"
#include "../Core/DependencyResolver.h"
#include "../Core/Events.h"
#include "../Core/Timer.h"

namespace Chilli {

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


	void ScriptApi::Init()
	{
		mono_add_internal_call("Chilli.InternalCalls::Log", Log);
		mono_add_internal_call("Chilli.InternalCalls::Input_IsKeyDown", Input_IsKeyDown);
	}
}