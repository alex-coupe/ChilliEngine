#include "ScriptApi.h"
#include "mono/jit/jit.h"
#include "../Core/DependencyResolver.h"
#include "../Core/Events.h"
#include "../Core/Timer.h"

namespace Engine::Scripting {

#pragma region Core

	static void Log(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		std::string message(str);
		mono_free(str);
		CHILLI_INFO(message);
	}

#pragma endregion

#pragma region Input

	static bool Input_IsKeyDown(Engine::Core::Key keycode)
	{
		return Engine::Core::DependencyResolver()
			.ResolveDependency<Engine::Core::Events>()->GetKeyPressed(keycode);
	}

#pragma endregion


	void Engine::Scripting::ScriptApi::Init()
	{
		mono_add_internal_call("Chilli.InternalCalls::Log", Log);
		mono_add_internal_call("Chilli.InternalCalls::Input_IsKeyDown", Input_IsKeyDown);
	}
}