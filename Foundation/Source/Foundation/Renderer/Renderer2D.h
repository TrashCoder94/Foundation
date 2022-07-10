#pragma once

#include "Camera.h"
#include "OrthographicCamera.h"
#include "SubTexture2D.h"

namespace Foundation
{
	class Model;

	class Renderer2D
	{
		public:
			struct Object2DProperties
			{
				Object2DProperties();

				// All rotations are in radians.

				// Coloured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& colour);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& colour);
				Object2DProperties(const glm::mat4& transform, const glm::vec4& colour);

				// ===================================
				// TEXTURES
				// Textured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture);
				
				// Tiled Textured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, float tilingFactor);
				
				// Textured Coloured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, const glm::vec4& colourTint);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, const glm::vec4& colourTint);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, const glm::vec4& colourTint);
				
				// Tiled Textured Coloured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& colourTint);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& colourTint);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& colourTint);
				
				// ===================================
				// SUBTEXTURES
				// Sub Textured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture);

				// Tiled Sub Textured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor);

				// Sub Textured Coloured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, const glm::vec4& colourTint);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, const glm::vec4& colourTint);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture, const glm::vec4& colourTint);

				// Tiled Sub Textured Coloured Quads
				Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& colourTint);
				Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& colourTint);
				Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& colourTint);

				glm::mat4 m_Transform;
				glm::vec4 m_Colour;
				SharedPtr<Texture2D> m_pTexture;
				SharedPtr<SubTexture2D> m_pSubTexture;
				float m_TextureTilingFactor;
			};

			static void Init();
			static void Shutdown();

			static void BeginScene(const Camera& camera, const glm::mat4& transform);
			static void BeginScene(const OrthographicCamera& orthographicCamera);	// TODO: Remove
			static void EndScene();
			static void Flush();

			static void FlushAndReset();

			// Primitives
			static void DrawQuad(const Object2DProperties& quadProperties = Object2DProperties());

			// Models
			static void DrawModel(const glm::mat4& transform, const SharedPtr<Model>& pModel);

			// Stats
			struct Statistics
			{
				uint32_t m_DrawCalls = 0;
				uint32_t m_QuadCount = 0;

				uint32_t GetTotalVertexCount() { return m_QuadCount * 4; }
				uint32_t GetTotalIndexCount() { return m_QuadCount * 6; }
			};
			static void ResetStats();
			static Statistics GetStats();
	};
}