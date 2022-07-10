#include "fdpch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <Reflect.h>

#include "Foundation/Components/CameraComponent.h"
#include "Foundation/Components/ModelComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Events/Event.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/Objects/CameraObject.h"
#include "Foundation/Renderer/Renderer2D.h"

namespace Foundation
{
	Scene::Scene(const std::string& name /*= "Untitled"*/) : BaseObject(),
		m_ID(),
		m_Name(name),
		m_pObjects(),
		m_pCurrentCameraObject(nullptr),
		m_pCurrentCamera(nullptr),
		m_CurrentCameraTransform(glm::mat4(1.0f)),
		m_ViewportWidth(0),
		m_ViewportHeight(0),
		m_Running(false)
	{
	}

	Scene::~Scene()
	{
		/*for (Object* pObject : m_pObjects)
		{
			if (!pObject)
				continue;

			pObject->End();
			delete pObject;
			pObject = nullptr;
		}

		m_pObjects.clear();
		m_pObjectMap.clear();*/
	}

	void Scene::Start()
	{
		BaseObject::Start();

		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
			m_pCurrentCameraObject->Start();

		auto func = [](Object* pObject)
		{
			pObject->Start();
		};
		IterateObjects(func);
	}

	void Scene::Update(float deltaTime)
	{
		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
			m_pCurrentCameraObject->Update(deltaTime);

		auto func = [&deltaTime](Object* pObject)
		{
			pObject->Update(deltaTime);
		};
		IterateObjects(func);
	}

	void Scene::End()
	{
		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
		{
			m_pCurrentCameraObject->End();
			m_pCurrentCameraObject = nullptr;
		}

		auto func = [](Object* pObject)
		{
			pObject->End();
		};
		IterateObjects(func);
	}

	Object* Scene::CreateObject(const std::string& name /*= "Object"*/)
	{
		Object* pObject = new Object();
		
		IDComponent* pIDComponent = pObject->AddComponent<IDComponent>();
		pIDComponent->m_UUID = {};

		pObject->AddComponent<TagComponent>(name);
		pObject->AddComponent<TransformComponent>(glm::vec3(0.0f));
		//pObject->AddComponent<ModelComponent>();
		pObject->SetOwner(this);
		pObject->Start();

		m_pObjects.push_back(pObject);

		return pObject;
	}
	
	const std::vector<Object*>& Scene::GetObjects() const
	{
		return m_pObjects;
	}

	const std::vector<Object*>& Scene::GetObjects()
	{
		return m_pObjects;
	}

	Object* Scene::CreateObjectWithID(UUID id, const std::string& name)
	{
		Object* pObject = new Object();

		IDComponent* pIDComponent = pObject->AddComponent<IDComponent>();
		pIDComponent->m_UUID = {};

		pObject->AddComponent<TagComponent>(name);
		pObject->AddComponent<TransformComponent>(glm::vec3(0.0f));
		pObject->AddComponent<ModelComponent>();
		pObject->SetOwner(this);
		pObject->Start();

		m_pObjects.push_back(pObject);

		return pObject;
	}

	void Scene::DestroyObject(Object* pObject)
	{
		for (size_t iO = m_pObjects.size(); iO > 0; --iO)
		{
			Object* pSceneObject = m_pObjects[iO];
			if (!pSceneObject)
				continue;

			if (pObject == pSceneObject)
			{
				pSceneObject->End();
				delete pSceneObject;
				pSceneObject = nullptr;

				m_pObjects.erase(m_pObjects.begin() + iO);
				break;
			}
		}
	}

	void Scene::Render()
	{
		// Camera
		if (CameraObject* pCurrentCamera = GetCurrentCamera())
		{
			m_pCurrentCamera = &pCurrentCamera->GetComponent<CameraComponent>()->m_Camera;
			m_CurrentCameraTransform = pCurrentCamera->GetComponent<TransformComponent>()->GetTransform();
		}

		if (m_pCurrentCamera)
		{
			Renderer2D::BeginScene(*m_pCurrentCamera, m_CurrentCameraTransform);
			{
				// Sprites
				auto spritesDrawFunction = [this](SpriteComponent* pSpriteComponent, TransformComponent* pTransformComponent)
				{
					if (!pSpriteComponent || !pTransformComponent)
						return;

					Renderer2D::DrawQuad({ pTransformComponent->GetTransform(), pSpriteComponent->m_pTexture, pSpriteComponent->m_TilingFactor, pSpriteComponent->m_Colour.GetColour() });
				};
				IterateObjectsWithComponents<SpriteComponent, TransformComponent>(spritesDrawFunction);

				// Models
				auto modelsDrawFunction = [this](ModelComponent* pModelComponent, TransformComponent* pTransformComponent)
				{
					if (!pModelComponent || !pTransformComponent)
						return;

					Renderer2D::DrawModel(pTransformComponent->GetTransform(), pModelComponent->m_pModel);
				};
				IterateObjectsWithComponents<ModelComponent, TransformComponent>(modelsDrawFunction);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResized(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		if (m_pCurrentCameraObject)
			m_pCurrentCameraObject->OnResize((float)m_ViewportWidth, (float)m_ViewportHeight);

		auto func = [this](CameraComponent* pCameraComponent)
		{
			if (!pCameraComponent)
				return;

			if (!pCameraComponent->m_FixedAspectRatio)
			{
				pCameraComponent->m_Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
				return;
			}
		};
		IterateObjectsWithComponent<CameraComponent>(func);
	}

	void Scene::OnEvent(Event& event)
	{
		if (m_pCurrentCameraObject)
			m_pCurrentCameraObject->OnEvent(event);

		auto func = [&event](Object* pObject)
		{
			pObject->OnEvent(event);
		};
		IterateObjects(func);
	}

	void Scene::SetCurrentCamera(CameraObject* pCurrentCamera)
	{
		m_pCurrentCameraObject = pCurrentCamera;
		m_pCurrentCamera = &m_pCurrentCameraObject->GetComponent<CameraComponent>()->m_Camera;
	}

	CameraObject* Scene::GetCurrentCamera()
	{
		return m_pCurrentCameraObject;
	}

	const CameraObject* Scene::GetCurrentCamera() const
	{
		return m_pCurrentCameraObject;
	}

	void Scene::Copy(const SharedPtr<Scene>& pScene)
	{
		m_ID = pScene->m_ID;
		m_Name = pScene->m_Name;

		auto func = [this](IDComponent* pSourceIDComponent)
		{
			UUID uuid = pSourceIDComponent->m_UUID;
			Object* pObject = CreateObjectWithID(uuid, "");
			m_pObjectMap[uuid] = pObject;
		};
		pScene->IterateObjectsWithComponent<IDComponent>(func);

		CopyComponents<TagComponent>(pScene->m_pObjects, m_pObjects);
		CopyComponents<TransformComponent>(pScene->m_pObjects, m_pObjects);
		CopyComponents<SpriteComponent>(pScene->m_pObjects, m_pObjects);
		CopyComponents<CameraComponent>(pScene->m_pObjects, m_pObjects);
	}

	void Scene::IterateObjects(std::function<void(Object*)> function)
	{
		for (Object* pObject : m_pObjects)
		{
			if (!pObject)
				continue;
			
			function(pObject);
		}
	}

	template<typename T>
	void Scene::CopyComponents(const std::vector<Object*>& pSourceObjects, std::vector<Object*>& pDestinationObjects)
	{
		for (Object* pSourceObject : pSourceObjects)
		{
			if (!pSourceObject)
				continue;

			if (Object* pDestinationObject = m_pObjectMap.at(pSourceObject->GetID()))
			{
				pDestinationObject->AddComponent<T>(*pSourceObject->GetComponent<T>());
			}
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Object* pObject, T* pComponent)
	{
		FD_CORE_ASSERT(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Object* pObject, IDComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Object* pObject, TagComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Object* pObject, TransformComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Object* pObject, SpriteComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Object* pObject, CameraComponent* pComponent)
	{
		pComponent->m_Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<typename T>
	void Scene::OnComponentRemoved(Object* pObject, T* pComponent)
	{
		FD_CORE_ASSERT(false);
	}

	template<>
	void Scene::OnComponentRemoved<IDComponent>(Object* pObject, IDComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentRemoved<TagComponent>(Object* pObject, TagComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentRemoved<TransformComponent>(Object* pObject, TransformComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentRemoved<SpriteComponent>(Object* pObject, SpriteComponent* pComponent)
	{
	}

	template<>
	void Scene::OnComponentRemoved<CameraComponent>(Object* pObject, CameraComponent* pComponent)
	{
	}

	template<typename T>
	void Scene::IterateObjectsWithComponent(std::function<void(T*)> function)
	{
		for (Object* pObject : m_pObjects)
		{
			if (!pObject)
				continue;

			if (T* pComponent = pObject->GetComponent<T>())
			{
				function(pComponent);
			}
		}
	}

	template<typename T, typename U>
	void Scene::IterateObjectsWithComponents(std::function<void(T*, U*)> function)
	{
		for (Object* pObject : m_pObjects)
		{
			if (!pObject)
				continue;

			if (T* pComponentOne = pObject->GetComponent<T>())
			{
				if (U* pComponentTwo = pObject->GetComponent<U>())
				{
					function(pComponentOne, pComponentTwo);
				}
			}
		}
	}
}