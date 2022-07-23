#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/EnemyManagerObject.h"
#include "Foundation/Components/TagComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Scene/Scene.h"
#include "Foundation/SpaceInvadersTest/BulletObject.h"
#include "Foundation/SpaceInvadersTest/EnemyObject.h"
#include "Foundation/SpaceInvadersTest/PlayerObject.h"
#include "Foundation/Renderer/Texture.h"

#include <imgui.h>

namespace Foundation
{
	EnemyManagerObject::EnemyManagerObject() : Object(),
		m_StartingPosition(glm::vec3(-4.0f, 2.5f, -7.0f)),
		m_StartingSize(glm::vec3(0.5f, 0.5f, 1.0f)),
		m_Spacing(glm::vec2(0.75f, 0.75f)),
		m_MinXPositionToTriggerDownMovement(-3.0f),
		m_MaxXPositionToTriggerDownMovement(3.0f),
		m_EnemyMoveSpeed(glm::vec2(0.005f, 0.005f)),
		m_pEnemySprite1(nullptr),
		m_pEnemySprite2(nullptr),
		m_EnemySpriteFrameDuration(1.0f),
		m_GameOverYPosition(-2.0f),
		m_NumberOfEnemiesPerRow(5),
		m_NumberOfRows(2),
		m_EnemyDeathParticleProperties(),
		m_NumberOfParticlesToEmitPerDeath(10),
		m_pPlayerObject(nullptr),
		m_SpawnPreview(false),
		m_MovingRight(true),
		m_SpawnedEnemies(false),
		m_PlayerWon(false),
		m_GameOver(false)
	{}
	
	EnemyManagerObject::~EnemyManagerObject()
	{}
	
	void EnemyManagerObject::Create()
	{
		Object::Create();
	}
	
	void EnemyManagerObject::Start()
	{
		Object::Start();

		if (Scene* pScene = GetScene())
		{
			for (Object* pObject : pScene->GetObjects())
			{
				if (!pObject)
				{
					continue;
				}

				if (pObject->Is<PlayerObject>())
				{
					m_pPlayerObject = static_cast<PlayerObject*>(pObject);
				}
			}
		}
	}
	
	void EnemyManagerObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);

		Scene* pScene = GetScene();
		if (!pScene)
		{
			return;
		}

		if (m_GameOver)
		{
			return;
		}

		if (m_PlayerWon)
		{
			return;
		}

		if (!m_SpawnedEnemies)
		{
			// Now that the scene has loaded
			// Create the enemies
			if (pScene->IsLoaded())
			{
				CreateEnemies();
				m_SpawnedEnemies = true;
			}
			return;
		}

		for(Object* pObject : pScene->GetObjects())
		{
			if (!pObject || !pObject->Is<EnemyObject>())
			{
				continue;
			}

			EnemyObject* pEnemy = static_cast<EnemyObject*>(pObject);
			if (pEnemy->IsDead())
			{
				continue;
			}

			if (TransformComponent* pEnemyTransformComponent = pEnemy->GetComponent<TransformComponent>())
			{
				if (pEnemyTransformComponent->m_Position.y - (pEnemyTransformComponent->m_Scale.y * 0.5f) <= m_GameOverYPosition)
				{
					FD_CORE_LOG_INFO("Enemy reached the bottom of the screen - GAME OVER!");
					m_GameOver = true;
					return;
				}

				if (m_MovingRight)
				{
					if (pEnemyTransformComponent->m_Position.x + (pEnemyTransformComponent->m_Scale.x * 0.5f) < m_MaxXPositionToTriggerDownMovement)
					{
						pEnemy->SetMovementMode(EnemyMovementMode::Right);
					}
					else
					{
						for (Object* pSceneObject : pScene->GetObjects())
						{
							if (!pSceneObject || !pSceneObject->Is<EnemyObject>())
							{
								continue;
							}

							EnemyObject* pEnemyObject = static_cast<EnemyObject*>(pSceneObject);
							if (pEnemyObject->IsDead())
							{
								continue;
							}

							pEnemyObject->SetMovementMode(EnemyMovementMode::Down);
						}
						m_MovingRight = false;
						break;
					}
				}
				else
				{
					if (pEnemyTransformComponent->m_Position.x - (pEnemyTransformComponent->m_Scale.x * 0.5f) > m_MinXPositionToTriggerDownMovement)
					{
						pEnemy->SetMovementMode(EnemyMovementMode::Left);
					}
					else
					{
						for (Object* pSceneObject : pScene->GetObjects())
						{
							if (!pSceneObject || !pSceneObject->Is<EnemyObject>())
							{
								continue;
							}

							EnemyObject* pEnemyObject = static_cast<EnemyObject*>(pSceneObject);
							if (pEnemyObject->IsDead())
							{
								continue;
							}

							pEnemyObject->SetMovementMode(EnemyMovementMode::Down);
						}
						m_MovingRight = true;
						break;
					}
				}
			}
		}

		if (m_pPlayerObject)
		{
			for (BulletObject* pBullet : m_pPlayerObject->GetBullets())
			{
				if (!pBullet || !pBullet->HasBeenFired())
				{
					continue;
				}

				bool bulletHasKilledSomething = false;
				for (Object* pObject : pScene->GetObjects())
				{
					if (!pObject || !pObject->Is<EnemyObject>())
					{
						continue;
					}

					EnemyObject* pEnemy = static_cast<EnemyObject*>(pObject);
					if (pEnemy->IsDead())
					{
						continue;
					}

					if (TransformComponent* pEnemyTransformComponent = pEnemy->GetComponent<TransformComponent>())
					{
						if (pBullet->IsColliding(pEnemyTransformComponent->m_Position, pEnemyTransformComponent->m_Scale))
						{
							bulletHasKilledSomething = OnEnemyKilledBy(pEnemy, pBullet);
							if (bulletHasKilledSomething)
							{
								break;
							}
						}
					}
				}

				if (bulletHasKilledSomething)
				{
					break;
				}
			}
		}
	}
	
	void EnemyManagerObject::End()
	{
		Object::End();
	}
	
	void EnemyManagerObject::Destroy()
	{
		Object::Destroy();
	}

	const bool EnemyManagerObject::IsGameOver()
	{
		return m_GameOver;
	}

	const bool EnemyManagerObject::HasPlayerWon()
	{
		return m_PlayerWon;
	}

	void EnemyManagerObject::CreateEnemies()
	{
		Scene* pScene = GetScene();
		if (!pScene)
		{
			return;
		}

		int numberOfEnemiesCreated = 0;
		glm::vec3 enemyPosition = m_StartingPosition;
		
		for (size_t iR = 0; iR < m_NumberOfRows; ++iR)
		{
			int currentRowNumber = (int)iR;
			std::vector<EnemyObject*> pEnemies;
			for (size_t iE = 0; iE < m_NumberOfEnemiesPerRow; ++iE)
			{
				const std::string& enemyName = "Enemy" + std::to_string(++numberOfEnemiesCreated);
				if (EnemyObject* pEnemyObject = pScene->CreateObject<EnemyObject>(enemyName))
				{
					pEnemyObject->GetComponent<TagComponent>()->m_Tag = enemyName;

					pEnemyObject->m_MoveSpeed = m_EnemyMoveSpeed;
					pEnemyObject->m_pEnemySprite1 = m_pEnemySprite1;
					pEnemyObject->m_pEnemySprite2 = m_pEnemySprite2;
					pEnemyObject->m_SpriteFrameDuration = m_EnemySpriteFrameDuration;

					pEnemyObject->GetComponent<SpriteComponent>()->m_pTexture = m_pEnemySprite1;

					TransformComponent* pEnemyTransformComponent = pEnemyObject->GetComponent<TransformComponent>();
					pEnemyTransformComponent->m_Position = enemyPosition;
					pEnemyTransformComponent->m_Scale = m_StartingSize;
					pEnemies.push_back(pEnemyObject);
					enemyPosition.x += m_Spacing.x;
				}
			}

			enemyPosition.x = m_StartingPosition.x;
			enemyPosition.y -= m_Spacing.y;
		}
	}

	bool EnemyManagerObject::OnEnemyKilledBy(EnemyObject* pEnemy, BulletObject* pBullet)
	{
		// If this enemy is already dead
		// We don't want to do anything with them.
		if (pEnemy->IsDead())
		{
			return false;
		}

		if (Scene* pScene = GetScene())
		{
			m_EnemyDeathParticleProperties.m_Position = pEnemy->GetComponent<TransformComponent>()->m_Position;
			m_EnemyDeathParticleProperties.m_Rotation = 0.0f;

			for (int iP = 0; iP < m_NumberOfParticlesToEmitPerDeath; ++iP)
			{
				pScene->GetParticleSystem().Emit(m_EnemyDeathParticleProperties);
			}

			pEnemy->SetIsDead(true);
			pBullet->Reload();
			pBullet->GetComponent<TransformComponent>()->m_Position = m_pPlayerObject->GetComponent<TransformComponent>()->m_Position;
			
			// TODO: Increment player score!

			bool areAllEnemiesDead = true;
			for (Object* pObject : pScene->GetObjects())
			{
				if (!pObject || !pObject->Is<EnemyObject>())
				{
					continue;
				}

				EnemyObject* pEnemyObject = static_cast<EnemyObject*>(pObject);
				areAllEnemiesDead &= pEnemyObject->IsDead();
				if (!areAllEnemiesDead)
				{
					break;
				}
			}

			if (areAllEnemiesDead)
			{
				m_PlayerWon = true;
				FD_CORE_LOG_INFO("Player has killed all of the enemies, they have won!");
			}
		}

		return true;
	}
}