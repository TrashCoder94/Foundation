#pragma once

#include "Foundation/Objects/Object.h"
#include "Foundation/Particles/ParticleSystem.h"

namespace Foundation
{
	class BulletObject;
	class EnemyObject;
	class PlayerObject;
	class Texture2D;

	class EnemyManagerObject : public Object
	{
		FD_REFLECT()

	public:
		EnemyManagerObject();
		~EnemyManagerObject();

		virtual void Create() override;
		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void End() override;
		virtual void Destroy() override;

		const bool IsGameOver();
		const bool HasPlayerWon();

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_StartingPosition;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_StartingSize;

		FVARIABLE(VariableFlags::Edit)
			glm::vec2 m_Spacing;

		FVARIABLE(VariableFlags::Edit)
			float m_MinXPositionToTriggerDownMovement;

		FVARIABLE(VariableFlags::Edit)
			float m_MaxXPositionToTriggerDownMovement;

		FVARIABLE(VariableFlags::Edit)
			glm::vec2 m_EnemyMoveSpeed;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pEnemySprite1;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pEnemySprite2;

		FVARIABLE(VariableFlags::Edit)
			float m_EnemySpriteFrameDuration;

		FVARIABLE(VariableFlags::Edit)
			int m_ScorePerEnemy;

		FVARIABLE(VariableFlags::Edit)
			float m_GameOverYPosition;

		FVARIABLE(VariableFlags::Edit)
			int m_NumberOfEnemiesPerRow;

		FVARIABLE(VariableFlags::Edit)
			int m_NumberOfRows;

		FVARIABLE(VariableFlags::Edit)
			ParticleProperties m_EnemyDeathParticleProperties;

		FVARIABLE(VariableFlags::Edit)
			int m_NumberOfParticlesToEmitPerDeath;

	private:
		void CreateEnemies();
		bool OnEnemyKilledBy(EnemyObject* pEnemy, BulletObject* pBullet);

		PlayerObject* m_pPlayerObject;

		bool m_SpawnPreview;
		bool m_MovingRight;
		bool m_SpawnedEnemies;
		bool m_PlayerWon;
		bool m_GameOver;
	};
}