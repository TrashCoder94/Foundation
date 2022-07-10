#include "fdpch.h"
#include "SpriteComponent.h"

namespace Foundation
{
	SpriteComponent::SpriteComponent(const Colour& colour, const float tilingFactor /*= 1.0f*/) : Component(),
		m_Colour(colour),
		m_pTexture(nullptr),
		m_TilingFactor(tilingFactor)
	{}

	SpriteComponent::~SpriteComponent()
	{
		m_pTexture.reset();
		m_pTexture = nullptr;
	}
}