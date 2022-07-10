#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Hides warnings from external includes.
#pragma warning(push, 0)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#pragma warning(pop)

#include "Component.h"

namespace Foundation
{
	class TransformComponent : public Component
	{
		FD_REFLECT()

	public:
		TransformComponent() = default;
		explicit TransformComponent(const TransformComponent&) = default;
		explicit TransformComponent(const glm::vec3& position);

		~TransformComponent();

		glm::mat4 GetTransform() const;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_Position;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_Rotation;
		
		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_Scale;
	};
}