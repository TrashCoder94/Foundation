#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/EnemyObject.h"
#include "Foundation/Core/Types.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Components/SpriteComponent.h"

namespace Foundation
{
	EnemyObject::EnemyObject() : Object(),
		m_MoveSpeed(0.5f),
		m_SpriteFrameDuration(0.25f),
		m_pEnemySprite1(nullptr),
		m_pEnemySprite2(nullptr),
		m_pTransformComponent(nullptr),
		m_pSpriteComponent(nullptr),
		m_CurrentAnimationFrame(0.0f),
		m_ShouldResetToSprite1OnNextFrame(false)
	{
		const Colour enemyColour = Colour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_pSpriteComponent = AddComponent<SpriteComponent>(enemyColour);
		
		static int s_EnemyCount = 0;
		TagComponent* pTagComponent = AddComponent<TagComponent>();
		pTagComponent->m_Tag = "Enemy" + std::to_string(s_EnemyCount++);
	}

	EnemyObject::~EnemyObject()
	{}
	
	void EnemyObject::Create()
	{
		Object::Create();
	}
	
	void EnemyObject::Start()
	{
		Object::Start();
		
		m_pTransformComponent = GetComponent<TransformComponent>();
		m_pSpriteComponent = GetComponent<SpriteComponent>();

		m_pSpriteComponent->m_pTexture = m_pEnemySprite1;

		SetMovementMode(EnemyMovementMode::Idle);
	}
	
	void EnemyObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);

		if (m_ShouldResetToSprite1OnNextFrame)
		{
			m_CurrentAnimationFrame += deltaTime;
			if (m_CurrentAnimationFrame >= m_SpriteFrameDuration)
			{
				if (m_pSpriteComponent)
				{
					m_pSpriteComponent->m_pTexture = m_pEnemySprite1;
					m_ShouldResetToSprite1OnNextFrame = false;
					m_CurrentAnimationFrame = 0.0f;
				}
			}
		}

		if (m_pTransformComponent)
		{
			switch (m_CurrentMovementMode)
			{
				case EnemyMovementMode::Idle:
				{
					break;
				}
				case EnemyMovementMode::Left:
				{
					m_pTransformComponent->m_Position.x -= (m_MoveSpeed * deltaTime);
					SetMovementMode(EnemyMovementMode::Idle);
					break;
				}
				case EnemyMovementMode::Right:
				{
					m_pTransformComponent->m_Position.x += (m_MoveSpeed * deltaTime);
					SetMovementMode(EnemyMovementMode::Idle);
					break;
				}
				case EnemyMovementMode::Down:
				{
					m_pTransformComponent->m_Position.y -= 1.0f; // constant downwards movement
					SetMovementMode(EnemyMovementMode::Idle);
					break;
				}
				default:
				{
					FD_CORE_ASSERT(false, "Movement mode not defined, please add a valid case statement here for it.");
					break;
				}
			}
		}
	}
	
	void EnemyObject::End()
	{
		Object::End();
	}
	
	void EnemyObject::Destroy()
	{
		Object::Destroy();
	}

	void EnemyObject::SetMovementMode(const EnemyMovementMode movementMode)
	{
		if (!m_pTransformComponent)
		{
			FD_CORE_LOG_ERROR("EnemyObject {0} doesn't have a valid TransformComponent, therefore, it cannot move.", GetComponent<TagComponent>()->m_Tag);
			return;
		}

		m_CurrentMovementMode = movementMode;

		switch (m_CurrentMovementMode)
		{
			case EnemyMovementMode::Idle:
			{
				if (m_pSpriteComponent)
				{
					m_pSpriteComponent->m_pTexture = m_pEnemySprite1;
					m_ShouldResetToSprite1OnNextFrame = false;
				}
				break;
			}
			case EnemyMovementMode::Left:
			case EnemyMovementMode::Right:
			case EnemyMovementMode::Down:
			{
				if (m_pSpriteComponent)
				{
					m_pSpriteComponent->m_pTexture = m_pEnemySprite2;
					m_ShouldResetToSprite1OnNextFrame = true;
				}
				break;
			}
			default:
			{
				FD_CORE_ASSERT(false, "Movement mode not defined, please add a valid case statement here for it.");
				break;
			}
		}
	}
}