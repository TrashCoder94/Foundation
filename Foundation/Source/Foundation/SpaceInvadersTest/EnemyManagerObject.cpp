#include "fdpch.h"
#include "Foundation/SpaceInvadersTest/EnemyManagerObject.h"
#include "Foundation/SpaceInvadersTest/EnemyObject.h"
#include "Foundation/Components/TagComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Scene/Scene.h"

#include <imgui.h>

namespace Foundation
{
	EnemyRowData::EnemyRowData() :
		m_pEnemyObject(nullptr),
		m_Row(0)
	{}

	EnemyManagerObject::EnemyManagerObject() : Object(),
		m_StartingPosition(glm::vec3(0.0f)),
		m_Spacing(glm::vec2(0.5f)),
		m_NumberOfEnemiesPerRow(4),
		m_NumberOfRows(3),
		m_EnemyRows(),
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
		}
	}
	
	void EnemyManagerObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);


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
}