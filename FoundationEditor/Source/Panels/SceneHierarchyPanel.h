#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <Foundation/Core/Base.h>
#include <Foundation/Core/UUID.h>
#include <Reflect.h>

namespace Foundation
{
	class Component;
	class BaseObject;
	class Object;
	class Scene;
	class Texture2D;

	class SceneHierarchyPanel
	{
		public:
			SceneHierarchyPanel();
			SceneHierarchyPanel(const SharedPtr<Scene>& pScene);

			void SetScene(const SharedPtr<Scene>& pScene);
			void OnImGuiRender();

			Object* GetSelectedObject() const { return m_pSelectedObject; }
		private:
			void DrawObjectNode(Object* pObject);
			void DrawComponentProperties(Object* pObject);

			void IterateAllMembers(BaseObject* pClassObject, const reflect::TypeDescriptor_Struct& reflectedData, const UUID& uniqueObjectID);
			void DrawMember(BaseObject* pClassObject, const reflect::TypeDescriptor_Struct::Member& member, const UUID& uniqueObjectID);

			void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
			void DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f);

		private:
			SharedPtr<Scene> m_pScene;
			SharedPtr<Texture2D> m_pCheckerboardTexture;
			Object* m_pSelectedObject;
	};
}