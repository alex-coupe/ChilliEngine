#include "EditorLayer.h"

Chilli::EditorLayer::EditorLayer()
{
	auto renderer = DependencyResolver::ResolveDependency<Renderer>();
	m_menuBar = std::make_shared<MenuBar>();
	m_scenePreview = std::make_shared<ScenePreview>();
	m_editorCamera = std::make_unique<Camera>(1.0f, renderer->GetAspectRatio(), 0.5f, 100.0f, CameraType::Editor, ProjectionType::Perspective);
	renderer->SetRenderCamera(m_editorCamera.get());
}

void Chilli::EditorLayer::OnUpdate()
{
	const auto sceneState = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetSceneState();
	
	switch (sceneState)
	{
	case SceneState::Edit:
		DependencyResolver::ResolveDependency<Renderer>()->SetRenderCamera(m_editorCamera.get());
		break;
	}
}

void Chilli::EditorLayer::OnRender()
{
	m_scenePreview->BindFrameBuffer();
}

void Chilli::EditorLayer::OnRenderGui()
{
	m_menuBar->DrawGui();
	m_scenePreview->DrawGui();
}

void Chilli::EditorLayer::OnResize()
{
	m_scenePreview->Resize();
}

float Chilli::EditorLayer::GetDisplayAspectRatio()
{
	return m_scenePreview->GetAspectRatio();
}
