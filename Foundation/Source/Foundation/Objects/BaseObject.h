#pragma once

#include <Reflect.h>

namespace Foundation
{
	class Event;

	class BaseObject
	{
		public:
			BaseObject() :
				m_pOwner(nullptr)
			{}

			virtual ~BaseObject()
			{
				m_pOwner = nullptr;
			}

			template<typename T>
			bool Is()
			{
				bool bIsSameType = (dynamic_cast<T*>(this) != nullptr);
				return bIsSameType;
			}

			virtual void Start() {}
			virtual void Update(float deltaTime) {}
			virtual void End() {}

			virtual void OnEvent(Event& event) {}

			virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;

			void SetOwner(BaseObject* pOwner) { m_pOwner = pOwner; }
			BaseObject* GetOwner() { return m_pOwner; }
			const BaseObject* GetOwner() const { return m_pOwner; }

		private:
			BaseObject* m_pOwner;
	};
}