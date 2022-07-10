#include <Foundation.h>
#include <Foundation/Core/EntryPoint.h>

#include "BuildMachineTestLayer.h"

class BuildMachineTest : public Foundation::Application
{
public:
	BuildMachineTest() : Foundation::Application()
	{
		PushLayer(new BuildMachineTestLayer());
	}

	~BuildMachineTest()
	{
	}
};

Foundation::Application* Foundation::CreateApplication()
{
	return new BuildMachineTest();
}