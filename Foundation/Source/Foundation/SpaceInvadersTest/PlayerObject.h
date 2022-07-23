#pragma once

#include "Foundation/Objects/Object.h"
#include "Foundation/Particles/ParticleSystem.h"
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

		void AddScore(const int scoreToAdd);
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

		FVARIABLE(VariableFlags::Edit)
			ParticleProperties m_PlayerWinParticleProperties;

		FVARIABLE(VariableFlags::Edit)
			int m_NumberOfParticlesToEmit;

		FVARIABLE(VariableFlags::Edit)
			float m_TimeBetweenWinParticles;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);

	private:
		std::vector<BulletObject*> m_pBullets;

		EnemyManagerObject* m_pEnemyManagerObject;

		TransformComponent* m_pTransformComponent;
		InputComponent* m_pInputComponent;
		SpriteComponent* m_pSpriteComponent;

		float m_CurrentAnimationFrame;
		float m_CurrentWinParticleTime;
		int m_CurrentScore;
		bool m_ShouldResetToSprite1OnNextFrame;
	};
}