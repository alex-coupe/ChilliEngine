#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "FrameBuffer.h"
#include "../ResourceSystem/ProjectManager.h"
#include "Drawable.h"
#include "Camera.h"

namespace Chilli {

	class CHILLI_API Renderer : public SubSystem{
	public:
		Renderer(int64_t width, int64_t height, void* handle);
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&)noexcept = default;
		const DirectX::XMMATRIX& GetProjectionMatrix()const;
		static SystemType GetSystemType();
		const std::unique_ptr<FrameBuffer>& GetFrameBuffer()const;
		bool Init();
		const std::unique_ptr<Camera>& GetEditorCamera();
		void ProcessFrame()override;
		const std::shared_ptr<Direct3D>& GetD3D()const;
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		float m_aspectRatio;
		std::shared_ptr<ProjectManager> m_sceneManager;
		DirectX::XMMATRIX m_projMatrix;
		std::vector<std::unique_ptr<Drawable>> m_drawables;
		std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_transformationCBuff;
		std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4>> m_color;
		std::unique_ptr<Camera> m_editorCamera;
		std::unique_ptr<FrameBuffer> m_frameBuffer;
	};
}