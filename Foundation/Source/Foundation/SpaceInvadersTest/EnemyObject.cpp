#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/EnemyObject.h"
#include "Foundation/SpaceInvadersTest/EnemyManagerObject.h"
#include "Foundation/Core/Types.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Scene/Scene.h"

namespace Foundation
{
	EnemyObject::EnemyObject() : Object(),
		m_MoveSpeed(glm::vec2(0.5f)),
		m_SpriteFrameDuration(1.0f),
		m_pEnemySprite1(nullptr),
		m_pEnemySprite2(nullptr),
		m_pEnemyManagerObject(nullptr),
		m_pTransformComponent(nullptr),
		m_pSpriteComponent(nullptr),
		m_CurrentMovementMode(EnemyMovementMode::Idle),
		m_CurrentAnimationFrame(0.0f),
		m_ShouldResetToSprite1OnNextFrame(false),
		m_IsDead(false)
	{
		const Colour enemyColour = Colour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_pSpriteComponent = AddComponent<SpriteComponent>(enemyColour);
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

		Scene* pScene = GetScene();
		if (!pScene)
		{
			return;
		}

		if (!pScene->IsLoaded())
		{
			return;
		}

		if (!m_pEnemyManagerObject)
		{
			for (Object* pObject : pScene->GetObjects())
			{
				if (!pObject || !pObject->Is<EnemyManagerObject>())
				{
					continue;
				}

				EnemyManagerObject* pEnemyManagerObject = static_cast<EnemyManagerObject*>(pObject);
				if (!m_pEnemyManagerObject)
				{
					m_pEnemyManagerObject = pEnemyManagerObject;
					break;
				}
			}
		}

		if (m_pEnemyManagerObject)
		{
			if (m_pEnemyManagerObject->IsGameOver() || m_pEnemyManagerObject->HasPlayerWon())
			{
				return;
			}
		}

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
					m_pTransformComponent->m_Position.x -= (m_MoveSpeed.x * deltaTime);
					break;
				}
				case EnemyMovementMode::Right:
				{
					m_pTransformComponent->m_Position.x += (m_MoveSpeed.x * deltaTime);
					break;
				}
				case EnemyMovementMode::Down:
				{
					m_pTransformComponent->m_Position.y -= m_MoveSpeed.y; // constant downwards movement
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

	void EnemyObject::Show()
	{
		if (m_pSpriteComponent)
		{
			Colour& spriteColour = m_pSpriteComponent->m_Colour;
			spriteColour.SetColour(glm::vec4(spriteColour.r(), spriteColour.g(), spriteColour.b(), 1.0f));
		}
	}

	void EnemyObject::Hide()
	{
		if (m_pSpriteComponent)
		{
			Colour& spriteColour = m_pSpriteComponent->m_Colour;
			spriteColour.SetColour(glm::vec4(spriteColour.r(), spriteColour.g(), spriteColour.b(), 0.0f));
		}
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
					if (!m_ShouldResetToSprite1OnNextFrame)
					{
						m_pSpriteComponent->m_pTexture = m_pEnemySprite2;
						m_ShouldResetToSprite1OnNextFrame = true;
					}
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
	bool EnemyObject::IsDead() const
	{
		return m_IsDead;
	}

	void EnemyObject::SetIsDead(const bool isDead)
	{
		m_IsDead = isDead;
		if (m_IsDead)
		{
			Hide();
		}
	}
}