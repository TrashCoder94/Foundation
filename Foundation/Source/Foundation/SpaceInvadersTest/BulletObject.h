#pragma once

#include "Foundation/Objects/Object.h"

#include <glm/glm.hpp>

namespace Foundation
{
	class SpriteComponent;
	class TransformComponent;

	class BulletObject : public Object
	{
		FD_REFLECT()

		friend class PlayerObject;

	public:
		BulletObject();
		~BulletObject();

		virtual void Create() override;
		virtual void Start() override;
		virtual void End() override;
		virtual void Update(float deltaTime) override;

		void Fire();
		void Reload();

		bool IsColliding(const glm::vec3& position, const glm::vec3& extent);
		bool HasBeenFired() const { return m_Fired; };

		FVARIABLE(VariableFlags::Edit)
			float m_Speed;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_Size;

	private:
		TransformComponent* m_pTransformComponent;
		SpriteComponent* m_pSpriteComponent;
		bool m_Fired;
	};
}