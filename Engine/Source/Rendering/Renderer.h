#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "FrameBuffer.h"
#include "../ResourceSystem/ProjectManager.h"
#include "RenderJob.h"
#include "Camera.h"

namespace Chilli {

	class CHILLI_API Renderer : public SubSystem{
	public:
		Renderer(int64_t width, int64_t height, void* handle);
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&)noexcept = default;
		static SystemType GetSystemType();
		const std::unique_ptr<FrameBuffer>& GetFrameBuffer()const;
		bool Init();
		void HandleResize(int64_t width, int64_t height);
		const std::unique_ptr<Camera>& GetEditorCamera();
		void ProcessFrame()override;
		const std::shared_ptr<Direct3D>& GetD3D()const;
	private:
		std::shared_ptr<Direct3D> m_direct3d;
		float m_aspectRatio;
		std::shared_ptr<ProjectManager> m_sceneManager;
		std::vector<RenderJob> m_renderJobs;
		std::unique_ptr<Camera> m_editorCamera;
		Camera* m_renderCamera;
		std::unique_ptr<FrameBuffer> m_frameBuffer;
	};
}