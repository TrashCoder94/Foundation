#pragma once

#include <glm/glm.hpp>

namespace Foundation
{
	class Colour
	{
		public:
			Colour();
			explicit Colour(const glm::vec4& colour);
			~Colour();

			glm::vec4& GetColour();
			const glm::vec4& GetColour() const;
			void SetColour(const glm::vec4 newColour);

			const float r() const;
			const float g() const;
			const float b() const;
			const float a() const;

		private:
			glm::vec4 m_Colour;
	};
}