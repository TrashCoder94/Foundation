#include "fdpch.h"
#include "Foundation/Components/InputComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Events/KeyEvent.h"
#include "Foundation/Scene/Scene.h"
#include "Foundation/SpaceInvadersTest/BulletObject.h"
#include "Foundation/SpaceInvadersTest/PlayerObject.h"
#include "Foundation/SpaceInvadersTest/EnemyManagerObject.h"
#include "Foundation/Core/Random.h"

namespace Foundation
{
	PlayerObject::PlayerObject() : Object(),
		m_MoveSpeed(0.5f),
		m_SpriteFrameDuration(0.25f),
		m_pPlayerSprite1(nullptr),
		m_pPlayerSprite2(nullptr),
		m_BulletSpeed(10.0f),
		m_BulletSize(glm::vec3(0.25, 0.5f, 1.0f)),
		m_PlayerWinParticleProperties(),
		m_NumberOfParticlesToEmit(10),
		m_TimeBetweenWinParticles(2.0f),
		m_pBullets(),
		m_pEnemyManagerObject(nullptr),
		m_pTransformComponent(nullptr),
		m_pInputComponent(nullptr),
		m_pSpriteComponent(nullptr),
		m_CurrentAnimationFrame(0.0f),
		m_CurrentWinParticleTime(0.0f),
		m_ShouldResetToSprite1OnNextFrame(false)
	{
		AddComponent<InputComponent>();
		
		const Colour playerColour = Colour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pSpriteComponent = AddComponent<SpriteComponent>(playerColour);
	}

	PlayerObject::~PlayerObject()
	{}

	void PlayerObject::Create()
	{
		Object::Create();
	}

	void PlayerObject::Start()
	{
		Object::Start();

		m_pTransformComponent = GetComponent<TransformComponent>();
		m_pInputComponent = GetComponent<InputComponent>();
		m_pSpriteComponent = GetComponent<SpriteComponent>();

		if (m_pInputComponent)
		{
			m_pInputComponent->BindInputFunction<KeyPressedEvent>(this, FD_BIND_EVENT_FN(PlayerObject::OnKeyPressed));
		}
		else
		{
			FD_CORE_LOG_ERROR("PlayerObject {0} doesn't have a valid InputComponent, please add one", GetComponent<TagComponent>()->m_Tag);
		}

		m_pBullets.push_back(new BulletObject());
		m_pBullets.push_back(new BulletObject());
		m_pBullets.push_back(new BulletObject());

		for (BulletObject* pBullet : m_pBullets)
		{
			if (!pBullet)
			{
				continue;
			}

			if (Scene* pScene = GetScene())
			{
				pBullet->m_pTransformComponent->m_Position = m_pTransformComponent->m_Position;
				pBullet->m_Speed = m_BulletSpeed;
				pBullet->m_Size = m_BulletSize;
				pScene->AddObject(pBullet, pBullet->GetComponent<TagComponent>()->m_Tag);
			}
		}

		m_pSpriteComponent->m_pTexture = m_pPlayerSprite1;

		if (Scene* pScene = GetScene())
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
	}

	void PlayerObject::Update(float deltaTime)
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
			if (m_pEnemyManagerObject->IsGameOver())
			{
				return;
			}

			if (m_pEnemyManagerObject->HasPlayerWon())
			{
				m_CurrentWinParticleTime += deltaTime;
				if (m_CurrentWinParticleTime >= m_TimeBetweenWinParticles)
				{
					const bool randXShouldBePositive = Random::Float() > 0.5f ? true : false;
					const bool randYShouldBePositive = Random::Float() > 0.5f ? true : false;
					const float randX = Random::Float() * (randXShouldBePositive ? 4.0f : -4.0f);
					const float randY = Random::Float() * (randYShouldBePositive ? 2.0f : -2.0f);
					const float z = -7.0f;
					const glm::vec3 randomParticlePosition = glm::vec3(randX, randY, z);
					m_PlayerWinParticleProperties.m_Position = randomParticlePosition;
					m_PlayerWinParticleProperties.m_Rotation = 0.0f;

					for (int iP = 0; iP < m_NumberOfParticlesToEmit; ++iP)
					{
						pScene->GetParticleSystem().Emit(m_PlayerWinParticleProperties);
					}
					m_CurrentWinParticleTime = 0.0f;
				}

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
					m_pSpriteComponent->m_pTexture = m_pPlayerSprite1;
					m_ShouldResetToSprite1OnNextFrame = false;
					m_CurrentAnimationFrame = 0.0f;
				}
			}
		}

		for (std::vector<BulletObject*>::iterator it = m_pBullets.begin(); it != m_pBullets.end(); ++it)
		{
			BulletObject* pBulletObject = (*it);
			if (!pBulletObject)
				continue;

			if (pBulletObject->m_pTransformComponent)
			{
				if (pBulletObject->m_Fired)
				{
					if (pBulletObject->m_pTransformComponent->m_Position.y >= 10.0f)
					{
						pBulletObject->Reload();
						pBulletObject->m_pTransformComponent->m_Position = m_pTransformComponent->m_Position;
					}
				}
				else
				{
					pBulletObject->m_pTransformComponent->m_Position = m_pTransformComponent->m_Position;
				}
			}
		}
	}

	void PlayerObject::End()
	{
		Object::End();

		for (std::vector<BulletObject*>::iterator it = m_pBullets.begin(); it != m_pBullets.end(); ++it)
		{
			BulletObject* pBulletObject = (*it);
			if (!pBulletObject)
				continue;

			if (Scene* pScene = GetScene())
			{
				pScene->RemoveObject(pBulletObject);

				pBulletObject->End();
				pBulletObject->Destroy();
				delete pBulletObject;
				pBulletObject = nullptr;
			}
		}

		m_pBullets.clear();
	}

	void PlayerObject::Destroy()
	{
		Object::Destroy();
	}

	bool PlayerObject::OnKeyPressed(KeyPressedEvent& event)
	{
		if (!m_pEnemyManagerObject)
		{
			return false;
		}

		if (m_pEnemyManagerObject->IsGameOver())
		{
			return false;
		}

		if (m_pEnemyManagerObject->HasPlayerWon())
		{
			return false;
		}

		if (event.GetKeyCode() == Key::Left || event.GetKeyCode() == Key::A)
		{
			m_pTransformComponent->m_Position.x -= m_MoveSpeed;
			m_pSpriteComponent->m_pTexture = m_pPlayerSprite2;
			m_ShouldResetToSprite1OnNextFrame = true;
		}
		else if (event.GetKeyCode() == Key::Right || event.GetKeyCode() == Key::D)
		{
			m_pTransformComponent->m_Position.x += m_MoveSpeed;
			m_pSpriteComponent->m_pTexture = m_pPlayerSprite2;
			m_ShouldResetToSprite1OnNextFrame = true;
		}

		if (event.GetKeyCode() == Key::Space)
		{
			TagComponent* pTagComponent = GetComponent<TagComponent>();

			for(BulletObject* pBullet : m_pBullets)
			{
				if (pBullet->m_Fired)
				{
					continue;
				}

				if (Scene* pScene = GetScene())
				{
					pBullet->m_pTransformComponent->m_Position = m_pTransformComponent->m_Position;
					pBullet->m_Speed = m_BulletSpeed;
					pBullet->Fire();
					break;
				}
			}
		}

		return false;
	}
}