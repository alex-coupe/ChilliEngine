#include "ScenePreview.h"
#include "../ChilliEditor.h"
using namespace DirectX;
Chilli::ScenePreview::ScenePreview()
{
	auto renderer = DependencyResolver::ResolveDependency<Renderer>();
	m_frameBuffer = std::make_unique<FrameBuffer>(renderer->GetD3D()->GetWindowWidth(), renderer->GetD3D()->GetWindowHeight(), renderer->GetD3D());
}

void Chilli::ScenePreview::BindFrameBuffer()
{
	m_frameBuffer->Bind();
}

void Chilli::ScenePreview::DrawGui(const std::unique_ptr<Camera>& editorCam)
{
	ImGui::Begin("Scene Preview", 0);

	const auto& renderer = DependencyResolver::ResolveDependency<Renderer>();
	auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();

	auto regionAvailable = ImGui::GetContentRegionAvail();

	if (regionAvailable.x == 0 || regionAvailable.y == 0)
	{
		ImGui::End();
		return;
	}

	if (m_scenePreviewWindowWidth != regionAvailable.x || m_scenePreviewWindowHeight != regionAvailable.y)
	{
		m_scenePreviewWindowWidth = regionAvailable.x;
		m_scenePreviewWindowHeight = regionAvailable.y;
		renderer->GetActiveCamera()->OnResize(regionAvailable.x, regionAvailable.y);
	}

	ImGui::Image(m_frameBuffer->GetShaderResourceView().Get(), regionAvailable);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_SCENE"))
		{
			const uint64_t* id = (const uint64_t*)payload->Data;
			DependencyResolver::ResolveDependency<ProjectManager>()->SetCurrentScene(*id);
		}
		ImGui::EndDragDropTarget();
	}

	if (projMan->GetCurrentScene() && projMan->GetCurrentScene()->GetSceneState() != SceneState::Play)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_1, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_2, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::ROTATE_X;
		if (ImGui::IsKeyPressed(ImGuiKey_3, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::ROTATE_Y;
		if (ImGui::IsKeyPressed(ImGuiKey_4, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::ROTATE_Z;
		if (ImGui::IsKeyPressed(ImGuiKey_5, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::SCALE;
		
		if (ImGui::IsKeyPressed(ImGuiKey_0, false) && ImGui::IsWindowHovered())
			m_guizmoType = -1;

		if (ChilliEditor::s_selectedEntity && m_guizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			auto pos = ImGui::GetWindowPos();
		
			ImGuizmo::SetRect(pos.x, pos.y, m_scenePreviewWindowWidth, m_scenePreviewWindowHeight);

			const auto& cameraProj = editorCam->GetProjMatrix();
			const auto& cameraView = editorCam->GetViewMatrix();
			const auto& tc = ChilliEditor::s_selectedEntity->GetTransformComponent();
			
			float tmpMat[16];
			ImGuizmo::RecomposeMatrixFromComponents(&tc->Translation().x, &tc->Rotation().x, &tc->Scale().x, tmpMat);
			
			DirectX::XMFLOAT4X4 projTemp;
			DirectX::XMFLOAT4X4 viewTemp;
			
			DirectX::XMStoreFloat4x4(&projTemp, cameraProj);
			DirectX::XMStoreFloat4x4(&viewTemp, cameraView);
			
			ImGuizmo::Manipulate(&viewTemp.m[0][0], &projTemp.m[0][0],
				(ImGuizmo::OPERATION)m_guizmoType, ImGuizmo::LOCAL, tmpMat);

			if (ImGuizmo::IsUsing())
			{
				float translation[3] = {0.0f,0.0f,0.0f};
				float rotation[3] = { 0.0f,0.0f,0.0f };
				float scale[3] = { 0.0f,0.0f,0.0f };

				ImGuizmo::DecomposeMatrixToComponents(tmpMat, translation, rotation, scale);
				tc->Translation() = { translation[0],translation[1],translation[2] };
				if (m_guizmoType == ImGuizmo::ROTATE_X)
					tc->Rotation().x += rotation[0] - tc->Rotation().x;
				if (m_guizmoType == ImGuizmo::ROTATE_Y)
					tc->Rotation().y += rotation[1] - tc->Rotation().y;
				if (m_guizmoType == ImGuizmo::ROTATE_Z)
					tc->Rotation().z += rotation[2] - tc->Rotation().z;
				
				tc->Scale() = { scale[0],scale[1],scale[2] };
			}
		}
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()
		&& projMan->GetCurrentScene()->GetSceneState() == SceneState::Edit)
	{
		auto mousePos = ImGui::GetMousePos();
		
		auto winPos = ImGui::GetWindowPos();
		auto clickableRegion = ImGui::GetContentRegionAvail();
		auto winSize = ImGui::GetWindowSize();
		auto padding = ImVec2({ clickableRegion.x - winSize.x,clickableRegion.y - winSize.y });
		auto normalised = ImVec2({ mousePos.x - winPos.x,mousePos.y-winPos.y });
		
		
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() 
		&& projMan->GetCurrentScene()->GetSceneState() == SceneState::Edit)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		if (m_initialMousePos)
		{
			auto winSize = ImGui::GetWindowSize();
			auto winPos = ImGui::GetWindowPos();
			m_mouseX = winPos.x + (winSize.x / 2);
			m_mouseY = winPos.y + (winSize.y / 2);
			m_initialMousePos = false;
		}
		else
		{
			auto mousePos = ImGui::GetMousePos();
			float xoffset = mousePos.x - m_mouseX;
			float yoffset = m_mouseY - mousePos.y;
			m_mouseX = mousePos.x;
			m_mouseY = mousePos.y;

			renderer->GetActiveCamera()->UpdateRotation(yoffset, xoffset);
		}
		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_W))
		{
			renderer->GetActiveCamera()->UpdatePosition(Direction::Forward);
		}

		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_A))
		{
			renderer->GetActiveCamera()->UpdatePosition(Direction::Left);
		}

		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_S))
		{
			renderer->GetActiveCamera()->UpdatePosition(Direction::Backward);
		}

		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_D))
		{
			renderer->GetActiveCamera()->UpdatePosition(Direction::Right);
		}
	}
	ImGui::End();
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		m_initialMousePos = true;
}

void Chilli::ScenePreview::Resize()
{
	m_frameBuffer.reset();
	auto renderer = DependencyResolver::ResolveDependency<Renderer>();
	m_frameBuffer = std::make_unique<FrameBuffer>(renderer->GetD3D()->GetWindowWidth(), renderer->GetD3D()->GetWindowHeight(), renderer->GetD3D());
}

float Chilli::ScenePreview::GetAspectRatio() const
{
	return m_scenePreviewWindowHeight / m_scenePreviewWindowWidth;
}

