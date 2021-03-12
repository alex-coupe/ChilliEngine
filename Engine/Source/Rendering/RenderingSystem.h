#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/EngineSystem.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

namespace Engine::Rendering {

	using namespace Engine::Core;

	class CHILLI_API RenderingSystem : public EngineSystem{
	public:
		RenderingSystem(std::shared_ptr<DependencyResolver<EngineSystem>> resolver, int64_t width, int64_t height, HWND handle);
		~RenderingSystem();
		int GetHash() override;
		bool Init();
		void ProcessFrame()override;
		void CreateTestCube();
	private:
		std::shared_ptr<Renderer> m_renderer;
	};
}