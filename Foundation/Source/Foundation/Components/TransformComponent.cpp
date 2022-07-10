#include "fdpch.h"
#include "TransformComponent.h"
#include "Foundation/Objects/Object.h"

namespace Foundation
{
	TransformComponent::TransformComponent(const glm::vec3& position) : Component(),
		m_Position(glm::vec3(0.0f)),
		m_Rotation(glm::vec3(0.0f)),
		m_Scale(glm::vec3(1.0f))
	{
	}

	TransformComponent::~TransformComponent()
	{}

	glm::mat4 TransformComponent::GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

		return 
		{ 
			glm::translate(glm::mat4(1.0f), m_Position)
			* rotation
			* glm::scale(glm::mat4(1.0f), m_Scale) 
		};
	}
}