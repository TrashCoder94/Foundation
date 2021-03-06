#pragma once

#include <Reflect.h>
#include "Foundation/Core/Log.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Foundation
{
	class Event;

	class BaseObject
	{
		public:
			BaseObject();
			virtual ~BaseObject();

			template<typename T>
			bool Is()
			{
				bool bIsSameType = (dynamic_cast<T*>(this) != nullptr);
				return bIsSameType;
			}

			void Copy(BaseObject* pSourceObject);

			virtual void Create() {}
			virtual void Start() {}
			virtual void Update(float deltaTime) {}
			virtual void End() {}
			virtual void Destroy() {}

			// For custom editor properties
			virtual void ImGuiRender() {}

			virtual void OnEvent(Event& event) {}

			virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;

			void SetOwner(BaseObject* pOwner) { m_pOwner = pOwner; }
			BaseObject* GetOwner() { return m_pOwner; }
			const BaseObject* GetOwner() const { return m_pOwner; }

		private:
			BaseObject* m_pOwner;
	};
}