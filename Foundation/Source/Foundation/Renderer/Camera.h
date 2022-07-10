#pragma once

#include <glm/glm.hpp>

namespace Foundation
{
	class Camera
	{
		public:
			Camera() :
				m_Projection(glm::mat4(1.0f))
			{}

			Camera(const glm::mat4& projection) :
				m_Projection(projection)
			{}
			virtual ~Camera() = default;

			const glm::mat4& GetProjection() const { return m_Projection; }

		protected:
			glm::mat4 m_Projection;
	};
}