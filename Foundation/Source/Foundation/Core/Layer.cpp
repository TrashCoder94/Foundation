#include "fdpch.h"
#include "Layer.h"

namespace Foundation
{
	Layer::Layer(const std::string& LayerName) :
		m_DebugName(LayerName)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::OnAttach()
	{
	}

	void Layer::OnDetach()
	{
	}

	void Layer::OnUpdate(Timestep ts)
	{
	}

	void Layer::OnRender()
	{
	}

	void Layer::OnImGuiRender()
	{
	}

	void Layer::OnEvent(Event& event)
	{
	}
}