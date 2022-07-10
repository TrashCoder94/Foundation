#pragma once

#include "Foundation/Core/Timestep.h"
#include "Foundation/Events/Event.h"

namespace Foundation
{
	class Layer
	{
		public:
			Layer(const std::string& LayerName = "Layer");
			virtual ~Layer();

			virtual void OnAttach();
			virtual void OnDetach();
			virtual void OnUpdate(Timestep ts);
			virtual void OnRender();
			virtual void OnImGuiRender();
			virtual void OnEvent(Event& event);

			inline const std::string& GetName() const { return m_DebugName; }
		protected:
			std::string m_DebugName;
	};
}
