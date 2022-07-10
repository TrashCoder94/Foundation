#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/Core/Types.h"

namespace Foundation
{
	class TestComponent : public Component
	{
		FD_REFLECT()

	public:
		TestComponent();
		~TestComponent();

		FVARIABLE(VariableFlags::Edit)
			Colour m_Colour;

		FVARIABLE(VariableFlags::Edit)
			glm::vec3 m_Position;
	};
}