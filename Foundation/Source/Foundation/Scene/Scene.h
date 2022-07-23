#pragma once

#include <functional>

#include "Foundation/Objects/BaseObject.h"
#include "Foundation/Renderer/Camera.h"
#include "Foundation/Core/UUID.h"
#include "Foundation/Particles/ParticleSystem.h"

#include "Foundation/Components/IDComponent.h"
#include "Foundation/Components/TagComponent.h"
#include "Foundation/Components/TransformComponent.h"

namespace Foundation
{
	class Event;
	class Object;
	class CameraObject;
	class SceneLoadedEvent;
	class SpriteComponent;

	class Scene : public BaseObject
	{
		FD_REFLECT()
			
		public:
			Scene(const std::string& name = "Untitled");
			~Scene();

			virtual void Create() override;
			virtual void Start() override;
			virtual void Update(float deltaTime) override;
			virtual void End() override;
			virtual void Destroy() override;

			virtual void ImGuiRender() override;

			void Render();

			Object* CreateObject(const std::string& name = "Object");
			void AddObject(Object* pObject, const std::string& name = "Object");

			void RemoveObject(Object* pObject);

			template<class T>
			T* CreateObject(const std::string& name = "Object")
			{
				T* pResult{ nullptr };

				if (!std::is_base_of<Object, T>::value)
					return pResult;

				T* pNewObject{ new T() };

				pNewObject->AddComponent<TagComponent>(name);

				IDComponent* pIDComponent = pNewObject->AddComponent<IDComponent>();
				pIDComponent->m_UUID = {};

				pNewObject->AddComponent<TransformComponent>(glm::vec3(0.0f));
				pNewObject->SetOwner(this);
				pNewObject->Start();

				if (!pNewObject)
					return pResult;

				pResult = pNewObject;
				m_pObjects.push_back(pResult);
				return pResult;
			}

			const std::vector<Object*>& GetObjects() const;
			const std::vector<Object*>& GetObjects();

			ParticleSystem& GetParticleSystem();

		private:
			Object* CreateObjectWithID(UUID id, const std::string& name = "Object");

		public:
			void DestroyObject(Object* pObject);

		public:
			void OnViewportResized(uint32_t width, uint32_t height);
			void OnEvent(Event& event);

			void SetCurrentCamera(CameraObject* pCurrentCamera);
			CameraObject* GetCurrentCamera();
			const CameraObject* GetCurrentCamera() const;

			void Copy(const SharedPtr<Scene>& pScene);

			UUID GetID() { return m_ID; }
			const UUID GetID() const { return m_ID; }
			const std::string& GetName() const { return m_Name; }
			const bool IsLoaded() const { return m_SceneLoaded; }

		private:			
			template<typename T>
			void CopyComponents(const std::vector<Object*>& pSourceObjects, std::vector<Object*>& pDestinationObjects);

			template<typename T>
			void OnComponentAdded(Object* pObject, T* pComponent);

			template<typename T>
			void OnComponentRemoved(Object* pObject, T* pComponent);

			void IterateObjects(const std::function<void(Object*)>& function);

			template<typename T>
			void IterateObjectsWithComponent(std::function<void(T*)> function);

			template<typename T, typename U>
			void IterateObjectsWithComponents(std::function<void(T*, U*)> function);

		private:
			UUID m_ID;
			std::string m_Name;
			std::vector<Object*> m_pObjects;
			CameraObject* m_pCurrentCameraObject;
			Camera* m_pCurrentCamera;
			glm::mat4 m_CurrentCameraTransform;
			uint32_t m_ViewportWidth;
			uint32_t m_ViewportHeight;
			bool m_Running;
			ParticleSystem m_ParticleSystem;
			bool m_SceneLoaded;

			friend class Object;
			friend class SceneSerializer;
			friend class SceneHierarchyPanel;
			friend class EditorLayer;
	};
}
