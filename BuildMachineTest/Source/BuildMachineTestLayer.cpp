#include "BuildMachineTestLayer.h"

BuildMachineTestLayer::BuildMachineTestLayer(float timeToTest /*= 5.0f*/) : Layer(),
	m_OrthographicCameraController(1920.0f / 1080.0f),
	m_TestTime(timeToTest),
	m_CurrentTestTime(timeToTest)
{}

BuildMachineTestLayer::~BuildMachineTestLayer()
{}

void BuildMachineTestLayer::OnAttach()
{

}

void BuildMachineTestLayer::OnDetach()
{

}

void BuildMachineTestLayer::OnUpdate(Foundation::Timestep ts)
{
	// Close down our application automatically if we have finished our countdown.
	m_CurrentTestTime -= ts;
	if (m_CurrentTestTime <= 0.0f)
	{
		Foundation::Application::Get().Close();
	}
}

void BuildMachineTestLayer::OnRender()
{

}

void BuildMachineTestLayer::OnImGuiRender()
{

}

void BuildMachineTestLayer::OnEvent(Foundation::Event& event)
{
	m_OrthographicCameraController.OnEvent(event);
}