#include <Foundation.h>
#include <Foundation/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Foundation::Application
{
public:
	Sandbox() : Foundation::Application()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Foundation::Application* Foundation::CreateApplication()
{
	return new Sandbox();
}