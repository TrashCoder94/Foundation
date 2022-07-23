#pragma once

#include "Foundation/Objects/Object.h"
#include "Foundation/Renderer/Texture.h"
#include "Foundation/SpaceInvadersTest/EnemyMovementModes.h"

namespace Foundation
{
	class EnemyManagerObject;
	class SpriteComponent;
	class TransformComponent;

	class EnemyObject : public Object
	{
		FD_REFLECT()

	public:
		EnemyObject();
		EnemyObject(const EnemyObject& enemyObject) = default;
		~EnemyObject();

		virtual void Create() override;
		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void End() override;
		virtual void Destroy() override;

		void Show();
		void Hide();
		void SetMovementMode(const EnemyMovementMode movementMode);
		bool IsDead() const;
		void SetIsDead(const bool isDead);

		FVARIABLE(VariableFlags::Edit)
			glm::vec2 m_MoveSpeed;

		FVARIABLE(VariableFlags::Edit)
			float m_SpriteFrameDuration;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pEnemySprite1;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pEnemySprite2;

	private:
		EnemyManagerObject* m_pEnemyManagerObject;
		TransformComponent* m_pTransformComponent;
		SpriteComponent* m_pSpriteComponent;

		EnemyMovementMode m_CurrentMovementMode;

		float m_CurrentAnimationFrame;
		bool m_ShouldResetToSprite1OnNextFrame;
		bool m_IsDead;
	};
}