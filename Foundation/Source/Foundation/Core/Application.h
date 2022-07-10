#pragma once

#include "Foundation/Core/Timestep.h"
#include "Foundation/Events/ApplicationEvent.h"
#include "Foundation/Core/LayerStack.h"
#include "Foundation/Core/Window.h"
#include "Foundation/Network/Client.h"
#include "Foundation/Core/PlayMode.h"

#include "Foundation/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Foundation
{
	class Application : public Client
	{
		public:
			Application(const std::string& name = "Foundation Application");
			virtual ~Application();

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

			void Close();

			void SetPlayMode(PlayMode newPlayMode);
 
			const PlayMode& GetPlayMode() { return m_PlayMode; }
			ImGuiLayer* GetImGuiLayer() { return m_pImGuiLayer; }
			Window& GetWindow() { return *m_pWindow; }

			static Application& Get() { return *s_pInstance; }
		private:
			void Run();
			void NetworkUpdate(Timestep ts);

			void OnEvent(Event& event);

			bool OnWindowClose(WindowCloseEvent& e);
			bool OnWindowResized(WindowResizeEvent& e);
			bool OnWindowFocus(WindowFocusEvent& e);
			bool OnWindowLostFocus(WindowLostFocusEvent& e);

			float GetTime();

			UniquePtr<Window> m_pWindow;
			ImGuiLayer* m_pImGuiLayer;
			PlayMode m_PlayMode;
			bool m_Running;
			bool m_Minimized;
			bool m_IsFocused;
			LayerStack m_LayerStack;
			Timestep m_Timestep;
			Timestep m_LastTimestep;
			float m_LastFrameTime;
			bool m_WaitForFullFocus;

			static Application* s_pInstance;

			// Used so we can keep Run() private and not expose it to client applications.
			// This may need to change if there are any updates to EntryPoint.h
			friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
