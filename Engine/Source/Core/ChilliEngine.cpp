#include "ChilliEngine.h"
#include <Windows.h>

void ChilliEngine::Update()
{
	while (m_window->Update())
	{
		m_timerSystem->ProcessFrame();
		m_eventSystem->ProcessFrame();
		m_renderingSystem->ProcessFrame();
	}
}

ChilliEngine::ChilliEngine(HINSTANCE& hInstance)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	//Create Systems
	if (m_resolver = std::make_shared<DependencyResolver<EngineSystem>>(); m_resolver == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Dependency Resolver", L"Chilli Error", MB_ICONWARNING | MB_OK);
		

	if (m_eventSystem = std::make_shared<EventSystem>(m_resolver); m_eventSystem == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Event System", L"Chilli Error", MB_ICONWARNING | MB_OK);
		

	if (m_timerSystem = std::make_shared<Timer>(m_resolver); m_timerSystem == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Timing System", L"Chilli Error", MB_ICONWARNING | MB_OK);
		
	if (m_window = std::make_unique<Window>(hInstance, m_eventSystem, false); m_window == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Create Window", L"Chilli Error", MB_ICONWARNING | MB_OK);
		

	m_renderingSystem = std::make_shared<RenderingSystem>(m_resolver, m_window->GetInitialWidth(), m_window->GetInitialHeight(), m_window->GetHandle());


	//Register Dependencies	
	m_resolver->Add(m_eventSystem);
	m_resolver->Add(m_timerSystem);
	m_resolver->Add(m_renderingSystem);

	//Initialize SubSystems As Required

	if (!m_renderingSystem->Init())
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Renderer", L"Chilli Error", MB_ICONWARNING |MB_ABORTRETRYIGNORE);
}

ChilliEngine::~ChilliEngine()
{
	m_renderingSystem.reset();
	m_resolver->Flush();
	m_resolver.reset();
	m_timerSystem.reset();
	m_window.reset();
	m_eventSystem.reset();
	ImGui::DestroyContext();
}




