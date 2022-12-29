#include "Application.h"

namespace Chilli {
	Application::Application()
	{
		m_engine = std::make_unique<ChilliEngine>();
		m_engine->SetAppLayer(std::make_shared<AppLayer>(m_engine->GetWindow()));
		auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
		projMan->LoadProject("C:\\Users\\dh211628.DHDOM1\\OneDrive - Dunnhumby Ltd\\Documents\\Projects\\2d Test\\2d Test.chilli");
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





