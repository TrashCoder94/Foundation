#pragma once

#include <glm/glm.hpp>
#include "Foundation/Core/Timestep.h"
#include "Foundation/Renderer/OrthographicCamera.h"

namespace Foundation
{
	struct ParticleProperties
	{
		public:
			glm::vec3 m_Position			= glm::vec3(1.0f);
			glm::vec3 m_Velocity			= glm::vec3(1.0f);
			glm::vec3 m_VelocityVariation	= glm::vec3(0.0f);
			glm::vec4 m_ColourBegin			= glm::vec4(1.0f);
			glm::vec4 m_ColourEnd			= glm::vec4(1.0f);
			glm::vec2 m_SizeBegin			= glm::vec2(1.0f);	// 2D Particles. TODO: Vec3 for 3D
			glm::vec2 m_SizeEnd				= glm::vec2(1.0f);	// 2D Particles. TODO: Vec3 for 3D
			glm::vec2 m_SizeVariation		= glm::vec2(0.0f);	// 2D Particles. TODO: Vec3 for 3D
			float m_Rotation				= 0.0f;				// 2D Particles. TODO: Vec3 for 3D
			float m_LifeTime				= 1.0f;
	};

	class ParticleSystem
	{
		public:
			ParticleSystem(uint32_t maxParticles = 1000);

			void Emit(const ParticleProperties& particleProps);

			void OnUpdate(Timestep ts);
			void OnRender(OrthographicCamera& orthographicCamera);

		private:
			struct Particle
			{
				glm::vec3 m_Position		= glm::vec3(1.0f);
				glm::vec3 m_Velocity		= glm::vec3(1.0f);
				glm::vec4 m_ColourBegin		= glm::vec4(1.0f);
				glm::vec4 m_ColourEnd		= glm::vec4(1.0f);
				glm::vec2 m_SizeBegin		= glm::vec2(1.0f);	// 2D Particles. TODO: Vec3 for 3D
				glm::vec2 m_SizeEnd			= glm::vec2(1.0f);	// 2D Particles. TODO: Vec3 for 3D
				float m_Rotation			= 0.0f;				// 2D Particles. TODO: Vec3 for 3D
				float m_LifeTime			= 1.0f;
				float m_LifeTimeRemaining	= 1.0f;

				bool m_IsActive			= false;
			};
			std::vector<Particle> m_ParticlePool;
			uint32_t m_PoolIndex;
	};
}