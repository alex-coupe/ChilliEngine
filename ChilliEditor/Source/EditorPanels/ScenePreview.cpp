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

	if (DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetSceneState() != SceneState::Play)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_1, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::TRANSLATE;

		if (ImGui::IsKeyPressed(ImGuiKey_2, false) && ImGui::IsWindowHovered())
			m_guizmoType = ImGuizmo::ROTATE;

		if (ImGui::IsKeyPressed(ImGuiKey_3, false) && ImGui::IsWindowHovered())
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
			const auto& transform = tc->GetTransformMatrix();
			DirectX::XMFLOAT4X4 projTemp;
			DirectX::XMFLOAT4X4 viewTemp;
			DirectX::XMFLOAT4X4 transformTemp;
			DirectX::XMStoreFloat4x4(&projTemp, cameraProj);
			DirectX::XMStoreFloat4x4(&viewTemp, cameraView);
			DirectX::XMStoreFloat4x4(&transformTemp, transform);

			ImGuizmo::Manipulate(&viewTemp.m[0][0], &projTemp.m[0][0],
				(ImGuizmo::OPERATION)m_guizmoType, ImGuizmo::LOCAL, &transformTemp.m[0][0]);

			if (ImGuizmo::IsUsing())
			{
				DirectX::XMVECTOR translation;
				DirectX::XMVECTOR rotation;
				DirectX::XMVECTOR scale;
				DirectX::XMMATRIX transforms = DirectX::XMMatrixSet(
					transformTemp.m[0][0], transformTemp.m[0][1], transformTemp.m[0][2], transformTemp.m[0][3],
					transformTemp.m[1][0], transformTemp.m[1][1], transformTemp.m[1][2], transformTemp.m[1][3],
					transformTemp.m[2][0], transformTemp.m[2][1], transformTemp.m[2][2], transformTemp.m[2][3],
					transformTemp.m[3][0], transformTemp.m[3][1], transformTemp.m[3][2], transformTemp.m[3][3]);
				DirectX::XMMatrixDecompose(&scale, &rotation, &translation, transforms);

				tc->Translation() = { DirectX::XMVectorGetX(translation),DirectX::XMVectorGetY(translation),DirectX::XMVectorGetZ(translation) };
				tc->RotQuart() = rotation;
				tc->Scale() = { DirectX::XMVectorGetX(scale),DirectX::XMVectorGetY(scale),DirectX::XMVectorGetZ(scale) };
			}
		}
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() 
		&& DependencyResolver::ResolveDependency<ProjectManager>
		()->GetCurrentScene()->GetSceneState() == SceneState::Edit)
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
