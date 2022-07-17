#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/Core/Types.h"
#include "Foundation/Renderer/Texture.h"

namespace Foundation
{
	class SpriteComponent : public Component
	{
		FD_REFLECT()

	public:
		SpriteComponent() = default;
		explicit SpriteComponent(const SpriteComponent&) = default;
		explicit SpriteComponent(const Colour& colour, const float tilingFactor = 1.0f);

		~SpriteComponent();

		FVARIABLE(VariableFlags::Edit)
			Colour m_Colour;

		FVARIABLE(VariableFlags::Edit)
			SharedPtr<Texture2D> m_pTexture;
		
		FVARIABLE(VariableFlags::Edit)
			float m_TilingFactor;
	};
}