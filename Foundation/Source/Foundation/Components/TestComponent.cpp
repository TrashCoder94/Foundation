#include "fdpch.h"
#include "TestComponent.h"

namespace Foundation
{
	TestComponent::TestComponent() : Component(),
		m_Colour(glm::vec4(1.0f)),
		m_Position(glm::vec3(0.0f))
	{}

	TestComponent::~TestComponent()
	{}
}