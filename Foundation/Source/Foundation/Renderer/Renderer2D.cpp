#include "fdpch.h"
#include "Renderer2D.h"

#include <glm/gtx/transform.hpp>

#include "Model.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Foundation
{
	/////////////////////////////////////////////////////////////////////////////////
	// Quad Vertex
	struct QuadVertex
	{
		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec4 m_Colour;
		float m_TexIndex;
		float m_TilingFactor;
	};

	/////////////////////////////////////////////////////////////////////////////////
	// Renderer 2D Storage
	struct Renderer2DData
	{
		// Batch Renderering - Max per draw call.
		static const uint32_t m_MaxNumberOfQuads = 20000;
		static const uint32_t m_MaxNumberOfVertices = m_MaxNumberOfQuads * 4;
		static const uint32_t m_MaxNumberOfIndices = m_MaxNumberOfQuads * 6;
		static const uint32_t m_MaxNumberOfTextureSlots = 32;

		SharedPtr<VertexArray> m_pQuadVertexArray;
		SharedPtr<VertexBuffer> m_pQuadVertexBuffer;
		SharedPtr<Shader> m_pTextureShader;
		SharedPtr<Shader> m_pModelShader;
		SharedPtr<Texture2D> m_pWhiteTexture;

		QuadVertex* m_pQuadVertexBufferBase = nullptr;
		QuadVertex* m_pQuadVertexBufferPtr = nullptr;
		uint32_t m_QuadIndexCount = 0;

		std::array<SharedPtr<Texture2D>, m_MaxNumberOfTextureSlots> m_TextureSlots;
		uint32_t m_TextureSlotIndex = 1;	// Slot 0 reserved for default 1x1 white texture.

		glm::vec4 m_QuadVertexPositions[4];

		Renderer2D::Statistics m_Stats;
	};

	// Explicity managing lifetime.
	// Certain Graphics API code needs to run whilst the graphics context is still alive when shutdown occurs.
	static Renderer2DData s_Data;

	/////////////////////////////////////////////////////////////////////////////////
	// Renderer 2D Object Properties
	Renderer2D::Object2DProperties::Object2DProperties() :
		m_Transform(glm::mat4(1.0f)),
		m_Colour(glm::vec4(1.0f)),
		m_pTexture(nullptr),
		m_TextureTilingFactor(1.0f)
	{
	}

	// Coloured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& colour) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, colour)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& colour) :
		Object2DProperties
		(
			{ 
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}) 
			},
			colour
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const glm::vec4& colour) :
		m_Transform(transform),
		m_Colour(colour),
		m_pTexture(nullptr),
		m_TextureTilingFactor(1.0f)
	{
	}

	// ===================================
	// TEXTURES
	// Textured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, texture)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			}, 
			texture, 
			1.0f, 
			glm::vec4(1.0f)
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture) :
		Object2DProperties(transform, texture, 1.0f, glm::vec4(1.0f))
	{
	}

	// Tiled Textured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, texture, tilingFactor)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			texture, 
			tilingFactor, 
			glm::vec4(1.0f)
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, float tilingFactor) :
		Object2DProperties(transform, texture, tilingFactor, glm::vec4(1.0f))
	{
	}

	// Textured Coloured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, const glm::vec4& colourTint) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, texture, colourTint)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, const glm::vec4& colourTint) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			texture, 
			1.0f, 
			colourTint
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, const glm::vec4& colourTint) :
		Object2DProperties(transform, texture, 1.0f, colourTint)
	{
	}

	// Tiled Textured Coloured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& colourTint) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, texture, tilingFactor, colourTint)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& colourTint) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			texture, 
			tilingFactor, 
			colourTint
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<Texture2D>& texture, float tilingFactor, const glm::vec4& colourTint) :
		m_Transform(transform),
		m_Colour(colourTint),
		m_pTexture(texture),
		m_pSubTexture(nullptr),
		m_TextureTilingFactor(tilingFactor)
	{
	}

	// ===================================
	// SUBTEXTURES
	// Textured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, subTexture)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			subTexture, 
			1.0f, 
			glm::vec4(1.0f)
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture) :
		Object2DProperties(transform, subTexture, 1.0f, glm::vec4(1.0f))
	{
	}

	// Tiled Textured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, subTexture, tilingFactor)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			subTexture, 
			tilingFactor, 
			glm::vec4(1.0f)
		)
	{
	}	
	
	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor) :
		Object2DProperties(transform, subTexture, tilingFactor, glm::vec4(1.0f))
	{
	}

	// Textured Coloured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, const glm::vec4& colourTint) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, subTexture, colourTint)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, const glm::vec4& colourTint) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			subTexture, 
			1.0f, 
			colourTint
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture, const glm::vec4& colourTint) :
		Object2DProperties(transform, subTexture, 1.0f, colourTint)
	{
	}

	// Tiled Textured Coloured Object
	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec2& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& colourTint) :
		Object2DProperties(glm::vec3(position.x, position.y, 1.0f), rotation, size, subTexture, tilingFactor, colourTint)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::vec3& position, float rotation, const glm::vec2& size, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& colourTint) :
		Object2DProperties
		(
			{
				  glm::translate(glm::mat4(1.0f), position)
				* ((rotation > 0.0f) ? glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) : glm::mat4(1.0f))
				* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			},
			subTexture,
			tilingFactor,
			colourTint
		)
	{
	}

	Renderer2D::Object2DProperties::Object2DProperties(const glm::mat4& transform, const SharedPtr<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& colourTint) :
		m_Transform(transform),
		m_Colour(colourTint),
		m_pTexture(nullptr),
		m_pSubTexture(subTexture),
		m_TextureTilingFactor(tilingFactor)
	{
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Renderer 2D
	void Renderer2D::Init()
	{
		FD_PROFILE_FUNCTION();

		// ============================================================
		// VERTEX ARRAYS

		// QUAD
		s_Data.m_pQuadVertexArray = VertexArray::Create();

		s_Data.m_pQuadVertexBuffer = VertexBuffer::Create(Renderer2DData::m_MaxNumberOfVertices * sizeof(QuadVertex));
		s_Data.m_pQuadVertexBuffer->SetLayout({
				{ Foundation::ShaderDataType::Float3, "m_Position" },
				{ Foundation::ShaderDataType::Float2, "m_TexCoord" },
				{ Foundation::ShaderDataType::Float4, "m_Colour" },
				{ Foundation::ShaderDataType::Float, "m_TexIndex" },
				{ Foundation::ShaderDataType::Float, "m_TilingFactor" }
			});
		s_Data.m_pQuadVertexArray->AddVertexBuffer(s_Data.m_pQuadVertexBuffer);

		s_Data.m_pQuadVertexBufferBase = new QuadVertex[Renderer2DData::m_MaxNumberOfVertices];

		uint32_t* quadIndices = new uint32_t[Renderer2DData::m_MaxNumberOfIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::m_MaxNumberOfIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		SharedPtr<Foundation::IndexBuffer> pQuadIndexBuffer = IndexBuffer::Create(quadIndices, Renderer2DData::m_MaxNumberOfIndices);
		s_Data.m_pQuadVertexArray->SetIndexBuffer(pQuadIndexBuffer);
		delete[] quadIndices;

		// ============================================================
		// TEXTURES

		// Creating a 1x1 texture to use as default for all renderering purposes.
		s_Data.m_pWhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.m_pWhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// ============================================================
		// SHADERS

		int32_t textureSamplers[Renderer2DData::m_MaxNumberOfTextureSlots];
		for (uint32_t iTS = 0; iTS < Renderer2DData::m_MaxNumberOfTextureSlots; ++iTS)
		{
			textureSamplers[iTS] = iTS;
		}

		s_Data.m_pTextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data.m_pTextureShader->Bind();
		s_Data.m_pTextureShader->SetIntArray("u_Textures", textureSamplers, Renderer2DData::m_MaxNumberOfTextureSlots);

		s_Data.m_pModelShader = Shader::Create("Assets/Shaders/Model.glsl");

		s_Data.m_TextureSlots[0] = s_Data.m_pWhiteTexture;

		const float quadVertexPosition = 0.5f;

		s_Data.m_QuadVertexPositions[0] = { -quadVertexPosition, -quadVertexPosition, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[1] = {  quadVertexPosition, -quadVertexPosition, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[2] = {  quadVertexPosition,  quadVertexPosition, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[3] = { -quadVertexPosition,  quadVertexPosition, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		FD_PROFILE_FUNCTION();

		delete[] s_Data.m_pQuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		FD_PROFILE_FUNCTION();

		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_Data.m_pTextureShader->Bind();
		s_Data.m_pTextureShader->SetMat4("u_ViewProjection", viewProjection);

		//s_Data.m_pModelShader->Bind();
		//s_Data.m_pModelShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.m_QuadIndexCount = 0;
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& orthographicCamera)
	{
		FD_PROFILE_FUNCTION();

		s_Data.m_pTextureShader->Bind();
		s_Data.m_pTextureShader->SetMat4("u_ViewProjection", orthographicCamera.GetViewProjectionMatrix());

		//s_Data.m_pModelShader->Bind();
		//s_Data.m_pModelShader->SetMat4("u_ViewProjection", orthographicCamera.GetViewProjectionMatrix());

		s_Data.m_QuadIndexCount = 0;
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		FD_PROFILE_FUNCTION();
		
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.m_pQuadVertexBufferPtr - (uint8_t*)s_Data.m_pQuadVertexBufferBase);
		s_Data.m_pQuadVertexBuffer->SetData(s_Data.m_pQuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		FD_PROFILE_FUNCTION();

		// Nothing to draw.
		if (s_Data.m_QuadIndexCount == 0)
		{
			return;
		}

		for (uint32_t iTS = 0; iTS < s_Data.m_TextureSlotIndex; ++iTS)
		{
			s_Data.m_TextureSlots[iTS]->Bind(iTS);
		}

		RenderCommand::DrawIndexed(s_Data.m_pQuadVertexArray, s_Data.m_QuadIndexCount);
		s_Data.m_Stats.m_DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.m_QuadIndexCount = 0;
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const Object2DProperties& quadProperties /*= QuadProperties()*/)
	{
		FD_PROFILE_FUNCTION();

		if (s_Data.m_Stats.m_QuadCount > s_Data.m_MaxNumberOfQuads)
		{
			FD_CORE_LOG_ERROR("Number of quads {0} exceeds maximum number of quads {1}, please delete some quads", s_Data.m_Stats.m_QuadCount, s_Data.m_MaxNumberOfQuads);
			return;
		}

		const SharedPtr<Texture2D> pTexture = quadProperties.m_pSubTexture ? quadProperties.m_pSubTexture->GetTexture() : quadProperties.m_pTexture;
		constexpr size_t quadVertexCount = 4;
		glm::vec2 textureCoords[4];

		// If we are attempting to use one small part of a larger sprite sheet.
		if (quadProperties.m_pSubTexture)
		{
			textureCoords[0] = quadProperties.m_pSubTexture->GetTexCoords()[0];
			textureCoords[1] = quadProperties.m_pSubTexture->GetTexCoords()[1];
			textureCoords[2] = quadProperties.m_pSubTexture->GetTexCoords()[2];
			textureCoords[3] = quadProperties.m_pSubTexture->GetTexCoords()[3];
		}
		// Otherwise, this is a single texture.
		else
		{
			textureCoords[0] = { 0.0f, 0.0f };
			textureCoords[1] = { 1.0f, 0.0f };
			textureCoords[2] = { 1.0f, 1.0f };
			textureCoords[3] = { 0.0f, 1.0f };
		}

		// 0 means use the default white texture, implies we just want to draw a coloured quad.
		float textureIndex = 0.0f;

		if (s_Data.m_QuadIndexCount >= s_Data.m_MaxNumberOfIndices)
		{
			FlushAndReset();
		}

		if (pTexture)
		{
			// Check to see if we have already assigned the texture to a texture slot.
			for (uint32_t iTS = 1; iTS < s_Data.m_TextureSlotIndex; ++iTS)
			{
				if (*s_Data.m_TextureSlots[iTS] == *pTexture)
				{
					textureIndex = (float)iTS;
					break;
				}
			}

			// If we didn't find the texture in our texture slots array.
			// Place it into our current batch.
			if (textureIndex == 0.0f)
			{
				if (s_Data.m_QuadIndexCount >= s_Data.m_MaxNumberOfIndices)
				{
					FlushAndReset();
				}

				textureIndex = (float)s_Data.m_TextureSlotIndex;
				s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = pTexture;
				s_Data.m_TextureSlotIndex++;
			}
		}

		for (size_t iQ = 0; iQ < quadVertexCount; ++iQ)
		{
			s_Data.m_pQuadVertexBufferPtr->m_Position = quadProperties.m_Transform * s_Data.m_QuadVertexPositions[iQ];
			s_Data.m_pQuadVertexBufferPtr->m_Colour = quadProperties.m_Colour;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = textureCoords[iQ];
			s_Data.m_pQuadVertexBufferPtr->m_TexIndex = textureIndex;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = quadProperties.m_TextureTilingFactor;
			s_Data.m_pQuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.m_QuadCount++;
	}

	void Renderer2D::DrawModel(const glm::mat4& transform, const SharedPtr<Model>& pModel)
	{
		if (!pModel)
		{
			return;
		}

		s_Data.m_pModelShader->SetMat4("u_Transform", transform);
		pModel->Draw(s_Data.m_pModelShader);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.m_Stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.m_Stats;
	}
}
