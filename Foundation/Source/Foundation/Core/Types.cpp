#include "fdpch.h"
#include "Types.h"

namespace Foundation
{
	Colour::Colour() :
		m_Colour(glm::vec4(1.0f))
	{}

	Colour::Colour(const glm::vec4& colour) : 
		m_Colour(colour)
	{}

	Colour::~Colour()
	{}

	glm::vec4& Colour::GetColour()
	{
		return m_Colour;
	}

	const glm::vec4& Colour::GetColour() const
	{
		return m_Colour;
	}

	void Colour::SetColour(const glm::vec4 newColour)
	{
		m_Colour = newColour;
	}

	const float Colour::r() const
	{
		return m_Colour.r;
	}

	const float Colour::g() const
	{
		return m_Colour.g;
	}

	const float Colour::b() const
	{
		return m_Colour.b;
	}

	const float Colour::a() const
	{
		return m_Colour.a;
	}
}