#include "fdpch.h"
#include "IDComponent.h"
#include "Foundation/Objects/Object.h"

namespace Foundation
{
	IDComponent::IDComponent(const UUID& id) : Component(),
		m_UUID(id)
	{}

	IDComponent::~IDComponent()
	{
	}
}