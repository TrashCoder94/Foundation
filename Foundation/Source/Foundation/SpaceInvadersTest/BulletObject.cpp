#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/BulletObject.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Core/Types.h"

#include <glm/glm.hpp>

namespace Foundation
{
	BulletObject::BulletObject() : Object(),
		m_Speed(1.0f),
		m_Size(glm::vec3(1.0f)),
		m_pTransformComponent(nullptr),
		m_Fired(false)
	{
		static int s_BulletCount = 0;

		m_pTransformComponent = AddComponent<TransformComponent>();
		m_pTransformComponent->m_Scale = glm::vec3(1.0f);

		TagComponent* pTagComponent = AddComponent<TagComponent>();
		pTagComponent->m_Tag = "Bullet" + std::to_string(s_BulletCount++);

		const Colour playerColour = Colour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSpriteComponent = AddComponent<SpriteComponent>(playerColour);
	}

	BulletObject::~BulletObject()
	{}

	void BulletObject::Create()
	{
		Object::Create();
	}

	void BulletObject::Start()
	{
		Object::Start();
		
		if (m_pTransformComponent)
		{
			m_pTransformComponent->m_Scale = m_Size;
		}
	}

	void BulletObject::End()
	{
		Object::End();
		m_Fired = false;
	}

	void BulletObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);

		if (!m_Fired)
		{
			return;
		}

		if (m_pTransformComponent)
		{
			m_pTransformComponent->m_Position.y += (m_Speed * deltaTime);
		}
	}

	void BulletObject::Fire()
	{
		m_Fired = true;

		if (m_pTransformComponent)
		{
			m_pTransformComponent->m_Scale = m_Size;
		}
	}

	void BulletObject::Reload()
	{
		m_Fired = false;
	}

	bool BulletObject::IsColliding(const glm::vec3& position, const glm::vec3& extent)
	{
		if (!m_pTransformComponent)
		{
			return false;
		}

		const glm::vec3& currentPosition = m_pTransformComponent->m_Position;

		// AABB
		const float halfXScale = (extent.x * 0.5f);
		const float halfYScale = (extent.y * 0.5f);
		const float maxXPosition = position.x + halfXScale;
		const float minXPosition = position.x - halfXScale;
		const float maxYPosition = position.y + halfYScale;
		const float minYPosition = position.y - halfYScale;

		// Check if this bullet is inside of the bounds given.
		if ((currentPosition.x < maxXPosition && currentPosition.x > minXPosition)
			&& (currentPosition.y < maxYPosition && currentPosition.y > minYPosition))
		{
			FD_CORE_LOG_INFO("BulletObject {0} is colliding with something", GetComponent<TagComponent>()->m_Tag);
			return true;
		}

		return false;
	}
}