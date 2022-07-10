#include "fdpch.h"
#include "CameraComponent.h"
#include "Foundation/Objects/Object.h"

namespace Foundation
{
	CameraComponent::CameraComponent() : Component(),
		m_Primary(false),
		m_FixedAspectRatio(false)
	{}

	CameraComponent::~CameraComponent()
	{}
}