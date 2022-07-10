#pragma once

#include <Foundation.h>

class BuildMachineTestLayer : public Foundation::Layer
{
public:
	BuildMachineTestLayer(float timeToTest = 5.0f);
	~BuildMachineTestLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Foundation::Timestep ts) override;
	void OnRender() override;
	void OnImGuiRender() override;
	void OnEvent(Foundation::Event& event) override;

private:
	Foundation::OrthographicCameraController m_OrthographicCameraController;

	float m_TestTime;
	float m_CurrentTestTime;
};