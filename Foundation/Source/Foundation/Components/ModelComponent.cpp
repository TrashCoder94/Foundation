#include "fdpch.h"
#include "ModelComponent.h"
#include "Foundation/Renderer/Model.h"

namespace Foundation
{
	ModelComponent::ModelComponent(const std::string filepath) : Component(),
		m_pModel(nullptr)
	{}

	ModelComponent::~ModelComponent()
	{
		m_pModel.reset();
		m_pModel = nullptr;
	}
}