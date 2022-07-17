#include "fdpch.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/Components/IDComponent.h"
#include "Foundation/Scene/Scene.h"

namespace Foundation
{
	Object::Object() : Object(nullptr)
	{}

	Object::Object(Scene* pScene) : BaseObject(),
		m_testFloat(10.0f),
		m_pComponents(),
		m_testInt(30)
	{}

	Object::~Object()
	{
	}

	void Object::Create()
	{
		BaseObject::Create();

		auto func = [](Component* pComponent)
		{
			pComponent->Create();
		};
		IterateComponents(func);
	}

	void Object::Start()
	{
		BaseObject::Start();

		auto func = [](Component* pComponent)
		{
			pComponent->Start();
		};
		IterateComponents(func);
	}

	void Object::Update(float deltaTime)
	{
		BaseObject::Update(deltaTime);

		auto func = [&deltaTime](Component* pComponent)
		{
			pComponent->Update(deltaTime);
		};
		IterateComponents(func);
	}

	void Object::End()
	{
		BaseObject::End();

		auto func = [](Component* pComponent)
		{
			pComponent->End();
		};
		IterateComponents(func);
	}

	void Object::Destroy()
	{
		BaseObject::Destroy();

		auto func = [](Component* pComponent)
		{
			pComponent->Destroy();
			delete pComponent;
			pComponent = nullptr;
		};
		IterateComponents(func);

		m_pComponents.clear();
	}

	void Object::OnEvent(Event& event)
	{
		BaseObject::OnEvent(event);

		auto func = [&event](Component* pComponent)
		{
			pComponent->OnEvent(event);
		};
		IterateComponents(func);
	}

	void Object::AddComponent(Component* pNewComponent)
	{
		if (!pNewComponent)
		{
			return;
		}

		if (HasComponent(pNewComponent))
		{
			const reflect::TypeDescriptor_Struct& componentInformation = pNewComponent->GetTypeDescription();
			FD_CORE_LOG_WARN("Object '{0}' already has a {1}",
				GetComponent<TagComponent>()->m_Tag,
				componentInformation.name);
			return;
		}

		pNewComponent->SetOwner(this);
		pNewComponent->Start();
		m_pComponents.push_back(pNewComponent);
	}

	bool Object::RemoveComponent(Component* pComponentToRemove)
	{
		bool componentRemoved = false;

		if (!pComponentToRemove)
		{
			return componentRemoved;
		}

		for (std::vector<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			Component* pComp = (*it);
			if (!pComp)
			{
				continue;
			}

			const reflect::TypeDescriptor_Struct& componentReflectedData = pComp->GetTypeDescription();
			const reflect::TypeDescriptor_Struct& componentToRemoveReflectedData = pComponentToRemove->GetTypeDescription();

			if (componentReflectedData.getFullName() == componentToRemoveReflectedData.getFullName())
			{
				m_pComponents.erase(it);
				componentRemoved = true;
				break;
			}
		}

		return componentRemoved;
	}

	bool Object::HasComponent(Component* pComponentToFind) const
	{
		bool bHasComponent = false;

		for (Component* pComponent : m_pComponents)
		{
			if (!pComponent)
				continue;

			const reflect::TypeDescriptor_Struct& componentReflectedData = pComponent->GetTypeDescription();
			const reflect::TypeDescriptor_Struct& componentToFindReflectedData = pComponentToFind->GetTypeDescription();

			if (componentReflectedData.getFullName() == componentToFindReflectedData.getFullName())
			{
				bHasComponent = true;
				break;
			}
		}

		return bHasComponent;
	}

	void Object::IterateComponents(std::function<void(Component*)> func)
	{
		for(std::vector<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			Component* pComponent = *it;
			if (!pComponent)
				continue;

			func(pComponent);
		}
	}

	const UUID Object::GetID() const
	{
		return GetComponent<IDComponent>()->m_UUID;
	}

	const UUID Object::GetSceneID() const
	{
		return GetScene()->GetID();
	}

	Scene* Object::GetScene()
	{
		return static_cast<Scene*>(GetOwner());
	}

	const Scene* Object::GetScene() const
	{
		return static_cast<const Scene*>(GetOwner());
	}

	const std::vector<Component*>& Object::GetComponents() const
	{
		return m_pComponents;
	}

	const std::vector<Component*>& Object::GetComponents()
	{
		return m_pComponents;
	}
}