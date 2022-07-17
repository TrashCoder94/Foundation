#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/Scene/SceneCamera.h"

namespace Foundation
{
	class CameraComponent : public Component
	{
		FD_REFLECT()

	public:
		CameraComponent();
		explicit CameraComponent(const CameraComponent&) = default;
		~CameraComponent();

		SceneCamera m_Camera;
		
		FVARIABLE(VariableFlags::Edit)
			bool m_Primary;

		FVARIABLE(VariableFlags::Edit)
			bool m_FixedAspectRatio;
	};
}