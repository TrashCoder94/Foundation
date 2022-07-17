#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/PlayerObject.h"
#include "Foundation/Components/InputComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Events/KeyEvent.h"

#include <glm/glm.hpp>

namespace Foundation
{
	PlayerObject::PlayerObject() : Object(),
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

	void PlayerObject::Start()
	{
		Object::Start();

		m_pTransformComponent = GetComponent<TransformComponent>();
		m_pInputComponent = GetComponent<InputComponent>();

		if (m_pInputComponent)
		{
			m_pInputComponent->BindInputFunction<KeyPressedEvent>(this, FD_BIND_EVENT_FN(PlayerObject::OnKeyPressed));
		}
		else
		{
			FD_CORE_LOG_ERROR("PlayerObject {0} doesn't have a valid InputComponent, please add one", GetComponent<TagComponent>()->m_Tag);
		}
	}

	void PlayerObject::End()
	{
		Object::End();
	}

	bool PlayerObject::OnKeyPressed(KeyPressedEvent & event)
	{
		if (event.GetKeyCode() == Key::Up || event.GetKeyCode() == Key::W)
			m_pTransformComponent->m_Position.z -= 0.5f;
		else if (event.GetKeyCode() == Key::Down || event.GetKeyCode() == Key::S)
			m_pTransformComponent->m_Position.z += 0.5f;

		if (event.GetKeyCode() == Key::Left || event.GetKeyCode() == Key::A)
			m_pTransformComponent->m_Position.x -= 0.5f;
		else if (event.GetKeyCode() == Key::Right || event.GetKeyCode() == Key::D)
			m_pTransformComponent->m_Position.x += 0.5f;

		return false;
	}
}