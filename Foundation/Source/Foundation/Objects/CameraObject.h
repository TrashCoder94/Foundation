#pragma once

#include <glm/glm.hpp>

#include "Foundation/Objects/Object.h"

namespace Foundation
{
	class Event;
	class Scene;
	class TransformComponent;
	class CameraComponent;
	class WindowResizeEvent;

	class CameraObject : public Object
	{
		public:
			CameraObject();
			CameraObject(Scene* pScene);
			~CameraObject();

			virtual void Start() override;
			virtual void Update(float deltaTime) override;
			virtual void End() override;

			virtual void OnEvent(Event& e) override;
			void OnResize(float width, float height);

		protected:
			CameraComponent* m_pCameraComponent;
			TransformComponent* m_pTransformComponent;

		private:
			bool OnWindowResized(WindowResizeEvent& event);
	};
}