#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "Direct3D.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

namespace Engine::Rendering {

	using namespace Engine::Core;

	class CHILLI_API Renderer : public SubSystem{
	public:
		Renderer(const std::shared_ptr<DependencyResolver<SubSystem>>& resolver, int64_t width, int64_t height, HWND handle);
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&)noexcept = default;
		int GetHash() const override;
		bool Init();
		void ProcessFrame()override;
		void CreateTestCube();
	private:
		std::shared_ptr<Direct3D> m_direct3d;
	};
}