#pragma once

#include "Foundation/Core/Layer.h"
#include "Foundation/Events/ApplicationEvent.h"
#include "Foundation/Events/KeyEvent.h"
#include "Foundation/Events/MouseEvent.h"

namespace Foundation
{
	class ImGuiLayer : public Layer
	{
		public:
			ImGuiLayer();
			~ImGuiLayer();

			void OnAttach() override;
			void OnDetach() override;
			void OnEvent(Event& e) override;

			void Begin();
			void End();

			void BlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }

			void SetDarkThemeColours();

		private:
			bool m_BlockEvents;
	};
}
