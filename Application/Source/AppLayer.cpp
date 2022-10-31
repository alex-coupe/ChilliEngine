#include "AppLayer.h"

Chilli::AppLayer::AppLayer()
{
	auto renderer = DependencyResolver::ResolveDependency<Renderer>();
	m_screenWidth = static_cast<float>(renderer->GetD3D()->GetWindowWidth());
	m_screenHeight = static_cast<float>(renderer->GetD3D()->GetWindowHeight());
}

void Chilli::AppLayer::OnUpdate()
{
}

void Chilli::AppLayer::OnRender()
{
	auto renderer = DependencyResolver::ResolveDependency<Renderer>();
	renderer->GetD3D()->SetBackBufferRenderTarget();
	renderer->GetD3D()->ClearBackBuffer();
}

void Chilli::AppLayer::OnRenderGui()
{
}

void Chilli::AppLayer::OnResize()
{
	auto renderer = DependencyResolver::ResolveDependency<Renderer>();
	m_screenWidth = static_cast<float>(renderer->GetD3D()->GetWindowWidth());
	m_screenHeight = static_cast<float>(renderer->GetD3D()->GetWindowHeight());
}

bool Chilli::AppLayer::HasGui()
{
	return false;
}

void Chilli::AppLayer::OnSceneChange()
{
}

float Chilli::AppLayer::GetDisplayAspectRatio()
{
	return m_screenHeight / m_screenWidth;
}
