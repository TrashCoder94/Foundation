#pragma once

#include <Reflect.h>

#include "Foundation/Objects/BaseObject.h"
#include "Foundation/Components/Component.h"
#include "Foundation/Components/TagComponent.h"
#include "Foundation/Core/UUID.h"

namespace Foundation
{
	class Scene;

	class Object : public BaseObject
	{
		FD_REFLECT()

		public:
			friend class Scene;
			friend class SceneSerializer;
			friend class SceneHierarchyPanel;

			Object();
			Object(Scene* pScene);
			Object(const Object& other) = default;

			~Object();

			virtual void Start() override;
			virtual void Update(float deltaTime) override;
			virtual void End() override;
			virtual void OnEvent(Event& event) override;

			template<class T, typename... Args>
			T* AddComponent(Args&&... args)
			{
				T* pResult{ nullptr };

				if (!std::is_base_of<Component, T>::value)
					return pResult;

				if (HasComponent<T>())
				{
					pResult = GetComponent<T>();
					const reflect::TypeDescriptor_Struct& componentInformation = pResult->GetTypeDescription();

					FD_CORE_LOG_WARN("Object '{0}' already has a {1}", 
						GetComponent<TagComponent>()->m_Tag,
						componentInformation.name);
					return pResult;
				}

				T* pNewComponent{ new T(std::forward<Args>(args)...) };
				pNewComponent->SetOwner(this);
				pNewComponent->Start();
				if (!pNewComponent)
					return pResult;

				pResult = pNewComponent;
				m_pComponents.push_back(pResult);

				return pResult;
			}

			void AddComponent(Component* pNewComponent);

			template<class T>
			T* GetComponent() const
			{
				T* pComponentResult = nullptr;

				if (!std::is_base_of<Component, T>::value)
					return pComponentResult;

				for (Component* pComponent : m_pComponents)
				{
					if (!pComponent)
						continue;

					if (pComponent->Is<T>())
					{
						pComponentResult = static_cast<T*>(pComponent);
						break;
					}
				}

				return pComponentResult;
			}

			template<class T>
			bool RemoveComponent()
			{	
				FD_CORE_ASSERT(HasComponent<T>(), "Object doesn't have component!");
				
				GetScene()->OnComponentRemoved<T>(this, GetComponent<T>());

				bool bRemovedComponent = false;

				for (size_t iC = m_pComponents.size() - 1; iC > 0; --iC)
				{
					Component* pComponent = m_pComponents[iC];
					if (!pComponent)
						continue;

					if (pComponent->Is<T>())
					{
						m_pComponents.erase(m_pComponents.begin() + iC);
						bRemovedComponent = true;
						break;
					}
				}

				return bRemovedComponent;
			}

			bool RemoveComponent(Component* pComponentToRemove);

			template<class T>
			bool HasComponent()
			{
				bool bHasComponent = false;

				if (!std::is_base_of<Component, T>::value)
					return bHasComponent;

				for (Component* pComponent : m_pComponents)
				{
					if (!pComponent)
						continue;

					if (pComponent->Is<T>())
					{
						bHasComponent = true;
						break;
					}
				}

				return bHasComponent;
			}

			bool HasComponent(Component* pComponentToFind) const;

			void IterateComponents(std::function<void(Component*)> func);

			const UUID GetID() const;
			const UUID GetSceneID() const;
						
			Scene* GetScene();
			const Scene* GetScene() const;

			const std::vector<Component*>& GetComponents() const;
			const std::vector<Component*>& GetComponents();

			bool operator==(const Object& other) const 
			{ 
				return GetID() == other.GetID() && GetScene() == other.GetScene();
			}

			bool operator!=(const Object& other) const 
			{ 
				return !(*this == other);
			}

		public:
			FVARIABLE(VariableFlags::Edit)
				float m_testFloat;
			
			FVARIABLE(VariableFlags::Edit)
				std::vector<Component*> m_pComponents;
			
			FVARIABLE(VariableFlags::Edit)
				int m_testInt;
	};
}