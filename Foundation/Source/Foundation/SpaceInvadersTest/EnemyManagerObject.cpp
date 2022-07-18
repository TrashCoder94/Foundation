#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/EnemyManagerObject.h"
#include "Foundation/Components/TagComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Scene/Scene.h"
#include "Foundation/SpaceInvadersTest/BulletObject.h"
#include "Foundation/SpaceInvadersTest/EnemyObject.h"
#include "Foundation/SpaceInvadersTest/PlayerObject.h"

#include <imgui.h>

namespace Foundation
{
	EnemyRowData::EnemyRowData() :
		m_pEnemyObject(nullptr),
		m_Row(0),
		m_IsDead(false)
	{}

	EnemyManagerObject::EnemyManagerObject() : Object(),
		m_StartingPosition(glm::vec3(-1.75f, 2.0f, -7.0f)),
		m_Spacing(glm::vec2(1.25f, 1.25f)),
		m_NumberOfEnemiesPerRow(4),
		m_NumberOfRows(3),
		m_EnemyDeathParticleProperties(),
		m_NumberOfParticlesToEmitPerDeath(5),
		m_EnemyRows(),
		m_pPlayerObject(nullptr),
		m_SpawnPreview(false)
	{}
	
	EnemyManagerObject::~EnemyManagerObject()
	{}
	
	void EnemyManagerObject::Create()
	{
		Object::Create();
	}
	
	void EnemyManagerObject::Start()
	{
		Object::Start();

		if (Scene* pScene = GetScene())
		{
			// Spawn enemies in the grid formation
			DestroyEnemies();
			CreateEnemies();

			// Start the enemies now we have spawned them in
			for (const EnemyRowData& enemyRow : m_EnemyRows)
			{
				if (enemyRow.m_pEnemyObject)
				{
					enemyRow.m_pEnemyObject->Start();
				}
			}

			for (Object* pObject : pScene->GetObjects())
			{
				if (!pObject)
				{
					continue;
				}

				if (pObject->Is<PlayerObject>())
				{
					m_pPlayerObject = static_cast<PlayerObject*>(pObject);
				}
			}
		}
	}
	
	void EnemyManagerObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);

		if (m_pPlayerObject)
		{
			for (BulletObject* pBullet : m_pPlayerObject->GetBullets())
			{
				if (!pBullet || !pBullet->HasBeenFired())
				{
					continue;
				}

				bool bulletHasKilledSomething = false;
				for (EnemyRowData& enemyRow : m_EnemyRows)
				{
					if (enemyRow.m_pEnemyObject)
					{
						if (TransformComponent* pEnemyTransformComponent = enemyRow.m_pEnemyObject->GetComponent<TransformComponent>())
						{
							if (pBullet->IsColliding(pEnemyTransformComponent->m_Position, pEnemyTransformComponent->m_Scale))
							{
								bulletHasKilledSomething = OnEnemyKilledBy(enemyRow, pBullet);
								if (bulletHasKilledSomething)
								{
									break;
								}
							}
						}
					}
				}

				if (bulletHasKilledSomething)
				{
					break;
				}
			}
		}
	}
	
	void EnemyManagerObject::End()
	{
		Object::End();
		DestroyEnemies();
	}
	
	void EnemyManagerObject::Destroy()
	{
		Object::Destroy();
	}
	
	void EnemyManagerObject::ImGuiRender()
	{
		Object::ImGuiRender();

		if (ImGui::Button("Preview Enemy Layout"))
		{
			if (m_SpawnPreview)
			{
				// Clear the enemies now
				m_SpawnPreview = false;
				DestroyEnemies();
			}
			else
			{
				// Spawn the enemies now
				m_SpawnPreview = true;
				CreateEnemies();
			}
		}
	}
	
	void EnemyManagerObject::CreateEnemies()
	{
		Scene* pScene = GetScene();
		if (!pScene)
		{
			return;
		}

		glm::vec3 enemyPosition = m_StartingPosition;

		for (size_t iR = 0; iR < m_NumberOfRows; ++iR)
		{
			int currentRowNumber = (int)iR;
			for (size_t iE = 0; iE < m_NumberOfEnemiesPerRow; ++iE)
			{
				EnemyRowData enemyRowData;
				enemyRowData.m_Row = currentRowNumber;
				enemyRowData.m_pEnemyObject = new EnemyObject();
				enemyRowData.m_pEnemyObject->Create();
			
				pScene->AddObject(enemyRowData.m_pEnemyObject, enemyRowData.m_pEnemyObject->GetComponent<TagComponent>()->m_Tag);

				TransformComponent* pEnemyTransformComponent = enemyRowData.m_pEnemyObject->GetComponent<TransformComponent>();
				pEnemyTransformComponent->m_Position = enemyPosition;

				m_EnemyRows.push_back(enemyRowData);

				enemyPosition.x += m_Spacing.x;
			}
			enemyPosition.x = m_StartingPosition.x;
			enemyPosition.y -= m_Spacing.y;
		}
	}
	
	void EnemyManagerObject::DestroyEnemies()
	{
		Scene* pScene = GetScene();
		if (!pScene || m_EnemyRows.size() == 0)
		{
			return;
		}

		for (EnemyRowData enemyRow : m_EnemyRows)
		{
			if (enemyRow.m_pEnemyObject)
			{
				pScene->RemoveObject(enemyRow.m_pEnemyObject);
				enemyRow.m_pEnemyObject->End();
				enemyRow.m_pEnemyObject->Destroy();

				delete enemyRow.m_pEnemyObject;
				enemyRow.m_pEnemyObject = nullptr;
			}
		}

		m_EnemyRows.clear();
	}

	bool EnemyManagerObject::OnEnemyKilledBy(EnemyRowData& enemyRowData, BulletObject* pBullet)
	{
		// If this enemy is already dead
		// We don't want to do anything with them.
		if (enemyRowData.m_IsDead)
		{
			return false;
		}

		if (Scene* pScene = GetScene())
		{
			m_EnemyDeathParticleProperties.m_Position = enemyRowData.m_pEnemyObject->GetComponent<TransformComponent>()->m_Position;
			m_EnemyDeathParticleProperties.m_Rotation = 0.0f;

			for (int iP = 0; iP < m_NumberOfParticlesToEmitPerDeath; ++iP)
			{
				pScene->GetParticleSystem().Emit(m_EnemyDeathParticleProperties);
			}

			enemyRowData.m_IsDead = true;

			pScene->RemoveObject(enemyRowData.m_pEnemyObject);
			enemyRowData.m_pEnemyObject->End();
			enemyRowData.m_pEnemyObject->Destroy();
			
			delete enemyRowData.m_pEnemyObject;
			enemyRowData.m_pEnemyObject = nullptr;
		}

		pBullet->Reload();
		pBullet->GetComponent<TransformComponent>()->m_Position = m_pPlayerObject->GetComponent<TransformComponent>()->m_Position;

		// TODO: Spawn particles!
		// TODO: Increment player score!
		
		return true;
	}
}