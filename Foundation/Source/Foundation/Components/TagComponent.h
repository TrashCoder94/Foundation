#pragma once

#include <string>

#include "Foundation/Components/Component.h"

namespace Foundation
{
	class TagComponent : public Component
	{
		FD_REFLECT()

	public:
		TagComponent() = default;
		explicit TagComponent(const TagComponent&) = default;
		explicit TagComponent(const std::string& tag);

		~TagComponent();

		FVARIABLE(VariableFlags::Edit)
			std::string m_Tag;
	};
}