#include "fdpch.h"
#include "ParticleSystem.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Foundation/Core/Random.h"
#include "Foundation/Renderer/Renderer2D.h"

namespace Foundation
{
	ParticleSystem::ParticleSystem(uint32_t maxParticles /*= 1000*/) :
		m_PoolIndex(maxParticles - 1)
	{
		m_ParticlePool.resize(maxParticles);
	}

	void ParticleSystem::Emit(const ParticleProperties& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.m_IsActive = true;

		// Position + Rotation
		particle.m_Position = particleProps.m_Position;
		particle.m_Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.m_Velocity = particleProps.m_Velocity;
		particle.m_Velocity.x += particleProps.m_VelocityVariation.x * (Random::Float() - 0.5f);
		particle.m_Velocity.y += particleProps.m_VelocityVariation.y * (Random::Float() - 0.5f);

		// Colour
		particle.m_ColourBegin = particleProps.m_ColourBegin;
		particle.m_ColourEnd = particleProps.m_ColourEnd;

		// Size
		particle.m_SizeBegin = particleProps.m_SizeBegin + (particleProps.m_SizeVariation * (Random::Float() - 0.5f));
		particle.m_SizeEnd = particleProps.m_SizeEnd;

		// Lifetime
		particle.m_LifeTime = particleProps.m_LifeTime;
		particle.m_LifeTimeRemaining = particleProps.m_LifeTime;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (Particle& particle : m_ParticlePool)
		{
			// Partice is not active.
			if (!particle.m_IsActive)
			{
				continue;
			}

			// Particle has died.
			if (particle.m_LifeTimeRemaining <= 0.0f)
			{
				particle.m_IsActive = false;
				continue;
			}

			particle.m_LifeTimeRemaining -= ts;
			particle.m_Position += particle.m_Velocity * (float)ts;
			particle.m_Rotation += 0.01f * ts; 
		}
	}

	void ParticleSystem::OnRender(OrthographicCamera& orthographicCamera)
	{
		Foundation::Renderer2D::BeginScene(orthographicCamera);
		{
			for (Particle& particle : m_ParticlePool)
			{
				// Partice is not active.
				if (!particle.m_IsActive)
				{
					continue;
				}

				float life = particle.m_LifeTimeRemaining / particle.m_LifeTime;
				glm::vec4 colour = glm::lerp(particle.m_ColourEnd, particle.m_ColourBegin, life);

				glm::vec2 size = glm::lerp(particle.m_SizeEnd, particle.m_SizeBegin, life);
				Renderer2D::DrawQuad({ particle.m_Position, glm::radians(particle.m_Rotation), size, colour });
			}
		}
		Foundation::Renderer2D::EndScene();
	}

	void ParticleSystem::OnRender(const Camera& camera, const glm::mat4& cameraTransform)
	{
		Renderer2D::BeginScene(camera, cameraTransform);
		{
			for (Particle& particle : m_ParticlePool)
			{
				// Partice is not active.
				if (!particle.m_IsActive)
				{
					continue;
				}

				float life = particle.m_LifeTimeRemaining / particle.m_LifeTime;
				glm::vec4 colour = glm::lerp(particle.m_ColourEnd, particle.m_ColourBegin, life);

				glm::vec2 size = glm::lerp(particle.m_SizeEnd, particle.m_SizeBegin, life);
				Renderer2D::DrawQuad({ particle.m_Position, glm::radians(particle.m_Rotation), size, colour });
			}
		}
		Renderer2D::EndScene();
	}
}