#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/Renderer/Model.h"

namespace Foundation
{
	class ModelComponent : public Component
	{
		FD_REFLECT()

	public:
		ModelComponent() = default;
		explicit ModelComponent(const ModelComponent&) = default;
		explicit ModelComponent(const std::string filepath);

		~ModelComponent();

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Model> m_pModel;
	};
}