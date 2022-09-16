#pragma once
#include "../Core/SubSystem.h"
#include "../Core/ChilliDefinitions.h"
#include <box2d/box2d.h>
#include "../Core/DependencyResolver.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Engine::Physics {

	class CHILLI_API PhysicsManager : public Engine::Core::SubSystem {
	public:
		PhysicsManager();
		virtual int GetSystemType()const override;
		virtual void ProcessFrame()override;
		b2Body* AddBody(const b2BodyDef& body)const;
	private:
		std::unique_ptr<b2World> m_b2World = nullptr;
		const float m_timestep = 1.0f / 60.0f;
		const int m_velocityIterations = 6;
		const int m_positionIterations = 2;
		const b2Vec2 m_gravity = { 0.0f, -1.0f };
	};
}