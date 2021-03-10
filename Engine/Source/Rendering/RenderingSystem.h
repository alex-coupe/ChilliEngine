#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/EngineSystem.h"
#include "Renderer.h"
#include "VertexBuffer.h"
namespace Engine::Rendering {

	using namespace Engine::Core;

	class CHILLI_API RenderingSystem : public Engine::Core::EngineSystem{
	public:
		RenderingSystem(HWND& handle, int64_t window_width, int64_t window_height,std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver);
		int GetHash() override;
		void Init();
		void ProcessFrame()override;
		void CreateTestTriangle();
	private:
		std::shared_ptr<Renderer> m_renderer;
	};
}