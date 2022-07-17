#include "fdpch.h"
#include "Foundation/Components/InputComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Events/KeyEvent.h"
#include "Foundation/Scene/Scene.h"
#include "Foundation/SpaceInvadersTest/BulletObject.h"
#include "Foundation/SpaceInvadersTest/PlayerObject.h"

namespace Foundation
{
	PlayerObject::PlayerObject() : Object(),
		m_BulletSpeed(1.0f),
		m_BulletSize(glm::vec3(1.0f)),
		m_pBullets(),
		m_pTransformComponent(nullptr),
		m_pInputComponent(nullptr),
		m_pSpriteComponent(nullptr)
	{
		AddComponent<InputComponent>();
		
		const Colour playerColour = Colour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
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

		if (m_pInputComponent)
		{
			m_pInputComponent->BindInputFunction<KeyPressedEvent>(this, FD_BIND_EVENT_FN(PlayerObject::OnKeyPressed));
			m_pInputComponent->BindInputFunction<KeyReleasedEvent>(this, FD_BIND_EVENT_FN(PlayerObject::OnKeyReleased));
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
	}

	void PlayerObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);

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
		if (event.GetKeyCode() == Key::Up || event.GetKeyCode() == Key::W)
			m_pTransformComponent->m_Position.z -= 0.5f;
		else if (event.GetKeyCode() == Key::Down || event.GetKeyCode() == Key::S)
			m_pTransformComponent->m_Position.z += 0.5f;

		if (event.GetKeyCode() == Key::Left || event.GetKeyCode() == Key::A)
			m_pTransformComponent->m_Position.x -= 0.5f;
		else if (event.GetKeyCode() == Key::Right || event.GetKeyCode() == Key::D)
			m_pTransformComponent->m_Position.x += 0.5f;

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

					TagComponent* pBulletTagComponent = pBullet->GetComponent<TagComponent>();
					const std::string& bulletName = pBulletTagComponent->m_Tag;
					if (pBulletTagComponent)
					{
						FD_CORE_LOG_INFO("PlayerObject {0} fired bullet {1}", pTagComponent->m_Tag, bulletName);
					}
					break;
				}
			}
		}

		return false;
	}

	bool PlayerObject::OnKeyReleased(KeyReleasedEvent& event)
	{
		

		return false;
	}
}