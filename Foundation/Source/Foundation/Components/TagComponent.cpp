#include "fdpch.h"
#include "TagComponent.h"
#include "Foundation/Objects/Object.h"

namespace Foundation
{
	TagComponent::TagComponent(const std::string& tag) : Component(),
		m_Tag(tag)
	{}

	TagComponent::~TagComponent()
	{
		m_Tag = "";
	}
}