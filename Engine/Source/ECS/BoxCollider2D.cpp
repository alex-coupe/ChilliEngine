#include "BoxCollider2D.h"

Engine::ECS::BoxCollider2D::BoxCollider2D(const Engine::Utilities::UUID& uuid)
	:Component(ComponentTypes::BoxCollider2D,"BoxCollider2D",uuid)
{
	b2PolygonShape box;
	box.SetAsBox(m_size.x, m_size.y);

}

const std::string Engine::ECS::BoxCollider2D::Serialize() const
{
	std::stringstream ss;
	return  ss.str();
}
