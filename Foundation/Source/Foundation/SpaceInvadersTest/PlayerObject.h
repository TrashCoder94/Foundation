#pragma once

#include "Foundation/Objects/Object.h"
#include "Foundation/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Foundation
{
	class BulletObject;
	class EnemyManagerObject;
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

		const std::vector<BulletObject*>& GetBullets() const { return m_pBullets; }

		FVARIABLE(VariableFlags::Edit)
			float m_MoveSpeed;

		FVARIABLE(VariableFlags::Edit)
			float m_SpriteFrameDuration;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pPlayerSprite1;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pPlayerSprite2;

		FVARIABLE(VariableFlags::Edit)
			float m_BulletSpeed;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_BulletSize;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);

	private:
		std::vector<BulletObject*> m_pBullets;

		TransformComponent* m_pTransformComponent;
		InputComponent* m_pInputComponent;
		SpriteComponent* m_pSpriteComponent;

		float m_CurrentAnimationFrame;
		bool m_ShouldResetToSprite1OnNextFrame;
	};
}