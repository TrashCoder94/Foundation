#pragma once

#include "Foundation/Objects/Object.h"
#include "Foundation/Renderer/Texture.h"
#include "Foundation/SpaceInvadersTest/EnemyMovementModes.h"

namespace Foundation
{
	class SpriteComponent;
	class TransformComponent;

	class EnemyObject : public Object
	{
		FD_REFLECT()

	public:
		EnemyObject();
		~EnemyObject();

		virtual void Create() override;
		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void End() override;
		virtual void Destroy() override;

		void SetMovementMode(const EnemyMovementMode movementMode);

		FVARIABLE(VariableFlags::Edit)
			float m_MoveSpeed;

		FVARIABLE(VariableFlags::Edit)
			float m_SpriteFrameDuration;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pEnemySprite1;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pEnemySprite2;

	private:
		TransformComponent* m_pTransformComponent;
		SpriteComponent* m_pSpriteComponent;

		EnemyMovementMode m_CurrentMovementMode;

		float m_CurrentAnimationFrame;
		bool m_ShouldResetToSprite1OnNextFrame;
	};
}