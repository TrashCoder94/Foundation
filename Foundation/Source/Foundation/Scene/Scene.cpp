#include "fdpch.h"
#include "Scene.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Reflect.h>

#include "Foundation/Core/Application.h"
#include "Foundation/Components/CameraComponent.h"
#include "Foundation/Components/ModelComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Events/Event.h"
#include "Foundation/Events/SceneEvent.h"
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
		m_Running(false),
		m_ParticleSystem(1000),
		m_SceneLoaded(false)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Create()
	{
		BaseObject::Create();

		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
		{
			m_pCurrentCameraObject->SetOwner(this);
			m_pCurrentCameraObject->Create();
		}

		auto func = [this](Object* pObject)
		{
			pObject->SetOwner(this);
			pObject->Create();
		};
		IterateObjects(func);
	}

	void Scene::Start()
	{
		BaseObject::Start();

		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
			m_pCurrentCameraObject->Start();

		auto func = [this](Object* pObject)
		{
			pObject->Start();
		};
		IterateObjects(func);

		m_SceneLoaded = true;
	}

	void Scene::Update(float deltaTime)
	{
		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
			m_pCurrentCameraObject->Update(deltaTime);

		auto func = [this, &deltaTime](Object* pObject)
		{
			pObject->Update(deltaTime);
		};
		IterateObjects(func);

		m_ParticleSystem.OnUpdate(deltaTime);
	}

	void Scene::End()
	{
		FD_PROFILE_FUNCTION();

		if (m_pCurrentCameraObject)
		{
			m_pCurrentCameraObject->End();
		}

		if (m_pObjects.size() > 0)
		{
			for (size_t iO = m_pObjects.size() - 1; iO > 0; --iO)
			{
				Object* pObject = m_pObjects.at(iO);
				if (!pObject)
				{
					continue;
				}

				pObject->End();
			}
		}

		m_SceneLoaded = false;
	}

	void Scene::Destroy()
	{
		BaseObject::Destroy();

		if (m_pCurrentCameraObject)
		{
			m_pCurrentCameraObject->Destroy();
			m_pCurrentCameraObject = nullptr;
		}

		for (size_t iO = m_pObjects.size() - 1; iO > 0; --iO)
		{
			Object* pObject = m_pObjects.at(iO);
			if (!pObject)
			{
				continue;
			}

			pObject->Destroy();
			delete pObject;
			pObject = nullptr;
		}

		m_pObjects.clear();
	}

	Object* Scene::CreateObject(const std::string& name /*= "Object"*/)
	{
		Object* pObject = new Object();
		
		IDComponent* pIDComponent = pObject->AddComponent<IDComponent>();
		pIDComponent->m_UUID = {};

		pObject->AddComponent<TagComponent>(name);
		pObject->AddComponent<TransformComponent>(glm::vec3(0.0f));
		pObject->SetOwner(this);
		pObject->Create();

		m_pObjects.push_back(pObject);

		return pObject;
	}

	void Scene::AddObject(Object* pObject, const std::string& name /*= "Object"*/)
	{
		if (!pObject)
		{
			FD_CORE_LOG_ERROR("Scene {0} couldn't add object because it was null", m_Name);
			return;
		}

		if (!pObject->HasComponent<IDComponent>())
		{
			IDComponent* pIDComponent = pObject->AddComponent<IDComponent>();
			pIDComponent->m_UUID = {};
		}
		if (!pObject->HasComponent<TagComponent>())
		{
			pObject->AddComponent<TagComponent>(name);
		}
		else
		{
			pObject->GetComponent<TagComponent>()->m_Tag = name;
		}
		if (!pObject->HasComponent<TransformComponent>())
		{
			pObject->AddComponent<TransformComponent>(glm::vec3(0.0f));
		}
		pObject->SetOwner(this);
		pObject->Create();

		m_pObjects.push_back(pObject);
	}

	void Scene::RemoveObject(Object* pObject)
	{
		if (!pObject)
		{
			return;
		}

		for (std::vector<Object*>::iterator it = m_pObjects.begin(); it != m_pObjects.end(); ++it)
		{
			if (*it == pObject)
			{
				m_pObjects.erase(it);
				break;
			}
		}
	}
	
	const std::vector<Object*>& Scene::GetObjects() const
	{
		return m_pObjects;
	}

	const std::vector<Object*>& Scene::GetObjects()
	{
		return m_pObjects;
	}

	ParticleSystem& Scene::GetParticleSystem()
	{
		return m_ParticleSystem;
	}

	Object* Scene::CreateObjectWithID(UUID id, const std::string& name)
	{
		Object* pObject = new Object();

		IDComponent* pIDComponent = pObject->AddComponent<IDComponent>();
		pIDComponent->m_UUID = {};

		pObject->AddComponent<TagComponent>(name);
		pObject->AddComponent<TransformComponent>(glm::vec3(0.0f));
		pObject->SetOwner(this);
		pObject->Create();

		m_pObjects.push_back(pObject);

		return pObject;
	}

	void Scene::DestroyObject(Object* pObject)
	{
		for (std::vector<Object*>::iterator it = m_pObjects.begin(); it != m_pObjects.end(); ++it)
		{
			Object* pSceneObject = (*it);

			if (!pSceneObject)
				continue;

			if (pObject == pSceneObject)
			{
				pSceneObject->End();
				delete pSceneObject;
				pSceneObject = nullptr;

				m_pObjects.erase(it);
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
			m_ParticleSystem.OnRender(*m_pCurrentCamera, m_CurrentCameraTransform);

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

	void Scene::ImGuiRender()
	{
		BaseObject::ImGuiRender();
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

		auto func = [this, &event](Object* pObject)
		{
			if (pObject)
			{
				pObject->OnEvent(event);
			}
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

		const std::vector<Object*>& pSceneObjects = pScene->m_pObjects;
		for (Object* pObject : pSceneObjects)
		{
			if (!pObject)
			{
				continue;
			}

			if (Object* pNewObject = new Object())
			{
				pNewObject->Copy(pObject);
				AddObject(pNewObject);
			}
		}
	}

	void Scene::IterateObjects(const std::function<void(Object*)>& function)
	{
		for(size_t iO = 0; iO < m_pObjects.size(); ++iO)
		{
			Object* pObject = m_pObjects[iO];
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