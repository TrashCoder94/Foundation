#include "fdpch.h"
#include "SubTexture2D.h"

namespace Foundation
{
	SubTexture2D::SubTexture2D(const SharedPtr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) :
		m_pTexture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	SharedPtr<SubTexture2D> SubTexture2D::Create(const SharedPtr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize /*= { 1.0f, 1.0f }*/)
	{
#if USE_PIXEL_COORDS
		glm::vec2 min = { coords.x / (float)texture->GetWidth(), coords.y / (float)texture->GetHeight() };
		glm::vec2 max = { (coords.x + cellSize.x) / (float)texture->GetWidth(), (coords.y + cellSize.y) / (float)texture->GetHeight() };
#else 
		float minX = coords.x == 0.0f ? 1.0f : coords.x;
		float minY = coords.y == 0.0f ? 1.0f : coords.y;

		glm::vec2 min = { (minX * cellSize.x) / (float)texture->GetWidth(), (minY * cellSize.y) / (float)texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / (float)texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / (float)texture->GetHeight() };
#endif
		SharedPtr<SubTexture2D> result = CreateSharedPtr<SubTexture2D>(texture, min, max);
		return result;
	}
}