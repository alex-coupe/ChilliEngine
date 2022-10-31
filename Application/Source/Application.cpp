#include "Application.h"

namespace Chilli {
	Application::Application()
	{
		m_engine = std::make_unique<ChilliEngine>();
		m_engine->SetAppLayer(std::make_shared<AppLayer>());
		auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
		projMan->LoadProject("Assets/Scenes/3d_test.chilli");
		projMan->PlayCurrentScene();

	}

	Application::~Application()
	{
		m_engine.reset();
	}

	void Application::Run()
	{
		return m_engine->Run();
	}
}





