#pragma once

#include "Foundation/Objects/Object.h"

namespace Foundation
{
	class BulletObject;
	class EnemyObject;
	class PlayerObject;

	struct EnemyRowData
	{
		EnemyRowData();
		EnemyObject* m_pEnemyObject;
		int m_Row;
		bool m_IsDead;
	};

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

		virtual void ImGuiRender() override;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_StartingPosition;

		FVARIABLE(VariableFlags::Edit)
			glm::vec2 m_Spacing;

		FVARIABLE(VariableFlags::Edit)
			int m_NumberOfEnemiesPerRow;

		FVARIABLE(VariableFlags::Edit)
			int m_NumberOfRows;

	private:
		void CreateEnemies();
		void DestroyEnemies();

		bool OnEnemyKilledBy(EnemyRowData& enemyRowData, BulletObject* pBullet);

		std::vector<EnemyRowData> m_EnemyRows;
		PlayerObject* m_pPlayerObject;

		bool m_SpawnPreview;
	};
}