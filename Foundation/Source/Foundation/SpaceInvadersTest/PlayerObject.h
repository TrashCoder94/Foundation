#pragma once

#include "Foundation/Objects/Object.h"

namespace Foundation
{
	class InputComponent;
	class KeyPressedEvent;
	class TransformComponent;
	class SpriteComponent;

	class PlayerObject : public Object
	{
		FD_REFLECT()

	public:
		PlayerObject();
		~PlayerObject();

		virtual void Start() override;
		virtual void End() override;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);

	private:
		TransformComponent* m_pTransformComponent;
		InputComponent* m_pInputComponent;
		SpriteComponent* m_pSpriteComponent;
	};
}