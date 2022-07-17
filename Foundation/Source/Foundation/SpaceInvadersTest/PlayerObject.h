#pragma once

#include "Foundation/Objects/Object.h"

#include <glm/glm.hpp>

namespace Foundation
{
	class BulletObject;
	class InputComponent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class TransformComponent;
	class SpriteComponent;

	class PlayerObject : public Object
	{
		FD_REFLECT()

	public:
		PlayerObject();
		~PlayerObject();

		virtual void Create() override;
		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void End() override;
		virtual void Destroy() override;

		FVARIABLE(VariableFlags::Edit)
			float m_BulletSpeed;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_BulletSize;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);

	private:
		std::vector<BulletObject*> m_pBullets;

		TransformComponent* m_pTransformComponent;
		InputComponent* m_pInputComponent;
		SpriteComponent* m_pSpriteComponent;
	};
}