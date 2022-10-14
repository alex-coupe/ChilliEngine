#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Chilli {
	class CHILLI_API Camera {
	public:
		Camera(DirectX::XMMATRIX projMatrix) :
			m_projMatrix(projMatrix)
		{}
		virtual ~Camera() {};
		virtual void OnResize(float newWidth, float newHeight) = 0;
		const DirectX::XMMATRIX& GetProjMatrix()const
		{
			return m_projMatrix;
		}
	protected:
		DirectX::XMMATRIX m_projMatrix;
	};
}