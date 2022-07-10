#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

#define USE_PIXEL_COORDS 1

namespace Foundation
{
	class SubTexture2D
	{
		public:
			SubTexture2D(const SharedPtr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

			const SharedPtr<Texture2D>& GetTexture() const { return m_pTexture; }
			const glm::vec2* GetTexCoords() const { return m_TexCoords; }

			static SharedPtr<SubTexture2D> Create(const SharedPtr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1.0f, 1.0f });

		private:
			SharedPtr<Texture2D> m_pTexture;
			glm::vec2 m_TexCoords[4];
	};
}