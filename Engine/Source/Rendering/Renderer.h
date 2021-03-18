#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "Direct3D.h"
#include "Primitives/Cube.h"
#include "../SceneManagement/SceneManager.h"
namespace Engine::Rendering {

	using namespace Engine::Core;
	using namespace Engine::SceneManagement;
	class CHILLI_API Renderer : public SubSystem{
	public:
		Renderer(const std::shared_ptr<DependencyResolver<SubSystem>>& resolver, int64_t width, int64_t height, HWND handle, const std::shared_ptr<Engine::Gui::GuiManager>& gui_man);
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&)noexcept = default;
		int GetHash() const override;
		bool Init();
		void ProcessFrame()override;
		void CreateTestCube();
		const std::shared_ptr<Direct3D>& GetD3D()const;
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		std::shared_ptr<Cube> m_testCube;
		std::shared_ptr<SceneManager> m_sceneManager;
	};
}