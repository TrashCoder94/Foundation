#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/Core/UUID.h"

namespace Foundation
{
	class IDComponent : public Component
	{
		FD_REFLECT()

		public:
			IDComponent() = default;
			explicit IDComponent(const IDComponent&) = default;
			explicit IDComponent(const UUID& id);

			~IDComponent();

			FVARIABLE(VariableFlags::Edit)
				UUID m_UUID;
	};
}