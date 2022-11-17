#include "EditorLayer.h"
#include "ChilliEditor.h"

namespace Chilli {
	
	EditorLayer::EditorLayer(std::unique_ptr<Window>& window)
		:m_window(window), Layer(LayerType::Editor)
	{
		ImGui::SetCurrentContext(GuiManager::GetContext());
		auto renderer = DependencyResolver::ResolveDependency<Renderer>();
		m_menuBar = std::make_shared<MenuBar>(window);
		m_scenePreview = std::make_shared<ScenePreview>();
		m_assetPanel = std::make_shared<AssetPanel>();
		m_sceneHierarchy = std::make_shared<SceneHierarchy>();
		m_entityInspector = std::make_shared<EntityInspector>();
		m_toolBar = std::make_shared<ToolBar>();
		m_splashPanel = std::make_shared<SplashPanel>();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Regular.ttf", 24.0f);
		io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Bold.ttf", 18.0f);

		m_editorCamera = std::make_unique<Camera>(1.0f, renderer->GetAspectRatio(), 0.5f, 100.0f, CameraType::Editor, ProjectionType::Perspective);
		renderer->SetRenderCamera(m_editorCamera.get());
	}

	void EditorLayer::OnOpen()
	{
		const auto& title = "Chilli Engine | " + DependencyResolver::ResolveDependency<ProjectManager>()->GetProjectName();
		m_window->SetTitle(title.c_str());
	}

	void EditorLayer::OnSceneChange()
	{
		ChilliEditor::s_selectedScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
	}

	void EditorLayer::OnUpdate()
	{
		const auto& currentScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
		if (currentScene)
		{
			switch (currentScene->GetSceneState())
			{
			case SceneState::Edit:
				DependencyResolver::ResolveDependency<Renderer>()->SetRenderCamera(m_editorCamera.get());
				break;
			}
		}
	}

	void EditorLayer::OnRender()
	{
		m_scenePreview->BindFrameBuffer();
	}

	void EditorLayer::OnRenderGui()
	{
		auto renderer = DependencyResolver::ResolveDependency<Renderer>();
		auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
		renderer->GetD3D()->SetBackBufferRenderTarget();
		renderer->GetD3D()->ClearBackBuffer();
		m_splashPanel->DrawGui();
		if (projMan->GetProjectOpen())
		{
			m_menuBar->DrawGui();
			m_scenePreview->DrawGui(m_editorCamera);
			m_assetPanel->DrawGui();
			m_sceneHierarchy->DrawGui();
			m_entityInspector->DrawGui();
			m_toolBar->DrawGui();
		}
		
	}

	void EditorLayer::OnResize()
	{
		m_scenePreview->Resize();
	}

	bool EditorLayer::HasGui()
	{
		return true;
	}

	float EditorLayer::GetDisplayAspectRatio()
	{
		return m_scenePreview->GetAspectRatio();
	}
}
