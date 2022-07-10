#include "fdpch.h"
#include "Application.h"

#include "Foundation/Core/Input.h"
#include "Foundation/Core/KeyCodes.h"
#include "Foundation/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <Reflect.h>

extern void reflect::InitApplicationReflection();

namespace Foundation
{
	Application* Application::s_pInstance = nullptr;

	Application::Application(const std::string& name /*= "Foundation Application"*/) :
		m_pWindow(nullptr),
		m_pImGuiLayer(nullptr),
		m_Running(true),
		m_Minimized(false),
		m_IsFocused(true),
		m_LayerStack(),
		m_Timestep(),
		m_LastTimestep(),
		m_LastFrameTime(0.0f),
		m_WaitForFullFocus(false),
		m_PlayMode(PlayMode::Offline)
	{
		FD_PROFILE_FUNCTION();

		FD_CORE_ASSERT(!s_pInstance, "Application already exists!");
		s_pInstance = this;

		m_pWindow = Window::Create(WindowProperties(name));
		m_pWindow->SetEventCallback(FD_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_pImGuiLayer = new ImGuiLayer();
		PushOverlay(m_pImGuiLayer);

		reflect::InitApplicationReflection();
	}

	Application::~Application()
	{
		FD_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		FD_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		FD_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::SetPlayMode(PlayMode newPlayMode)
	{
		m_PlayMode = newPlayMode;

		switch (m_PlayMode)
		{
			case PlayMode::Server:
			{
				break;
			}
			case PlayMode::Client:
			{
				// TODO: Setup parameters for this?
				Connect("127.0.0.1", 60000);
				break;
			}
			case PlayMode::Offline:
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		FD_PROFILE_FUNCTION();

		while (m_Running)
		{
			FD_PROFILE_SCOPE("RunLoop");

			if (m_IsFocused)
			{
				float time = GetTime();
				m_LastTimestep = m_Timestep;
				m_Timestep = time - m_LastFrameTime;
				m_LastFrameTime = time;

				if (!m_Minimized)
				{
					{
						FD_PROFILE_SCOPE("LayerStack OnUpdate");

						for (Layer* layer : m_LayerStack)
						{
							layer->OnUpdate(m_Timestep);
						}
					}

					if(m_PlayMode == PlayMode::Client)
					{
						FD_PROFILE_SCOPE("NetworkUpdate");
						NetworkUpdate(m_Timestep);
					}

					{
						FD_PROFILE_SCOPE("LayerStack OnRender");

						for (Layer* layer : m_LayerStack)
						{
							layer->OnRender();
						}
					}
				}
			}

			m_pImGuiLayer->Begin();
			{
				FD_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_pImGuiLayer->End();

			m_pWindow->OnUpdate();
		}
	}

	void Application::NetworkUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::D1))
			PingServer();
		if (Input::IsKeyPressed(Key::D2))
			MessageAll();
		if (Input::IsKeyPressed(Key::D3))
			m_Running = false;

		if (IsConnected())
		{
			if (!GetIncomingMessages().empty())
			{
				auto message = GetIncomingMessages().pop_front().m_Message;

				switch (message.m_Header.m_ID)
				{
					case NetMessageTypes::ServerAccept:
					{
						FD_CORE_LOG_INFO("[CLIENT] - Server accepted connection");
						break;
					}
					case NetMessageTypes::ServerDeny:
					{
						break;
					}
					case NetMessageTypes::ServerPing:
					{
						std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point timeThen;
						message >> timeThen;
						FD_CORE_LOG_INFO("[CLIENT] - Ping: ['{0}']", std::chrono::duration<double>(timeNow - timeThen).count());
						break;
					}
					case NetMessageTypes::ServerMessage:
					{
						uint32_t clientID;
						message >> clientID;
						FD_CORE_LOG_INFO("[CLIENT] - Hello from ['{0}']", clientID);
						break;
					}
					case NetMessageTypes::MessageAll:
					{
						break;
					}
				}
			}
		}
		else
		{
			FD_CORE_LOG_INFO("[CLIENT] - Server Down");
			Close();
		}
	}

	void Application::OnEvent(Event& event)
	{
		FD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(FD_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(FD_BIND_EVENT_FN(Application::OnWindowResized));
		dispatcher.Dispatch<WindowFocusEvent>(FD_BIND_EVENT_FN(Application::OnWindowFocus));
		dispatcher.Dispatch<WindowLostFocusEvent>(FD_BIND_EVENT_FN(Application::OnWindowLostFocus));

		// Work backwards in the layers so that we start with overlays first.
		// If an overlay handles an event, the standard layers will not receive that event.
		for (std::vector<Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (event.IsHandled())
			{
				break;
			}
			(*--it)->OnEvent(event);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		FD_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnWindowFocus(WindowFocusEvent& e)
	{
		m_IsFocused = true;
		return false;
	}

	bool Application::OnWindowLostFocus(WindowLostFocusEvent& e)
	{
		m_IsFocused = false;
		m_WaitForFullFocus = true;
		return false;
	}

	float Application::GetTime()
	{
		float time{ (float)glfwGetTime() };
		if (m_WaitForFullFocus)
		{
			m_LastFrameTime = time - m_LastTimestep;
			m_WaitForFullFocus = false;
		}
		return time;
	}
}