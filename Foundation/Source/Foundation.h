#pragma once

// Only to be used by Foundation applications!

#include "Foundation/Core/Application.h"
#include "Foundation/Core/Layer.h"
#include "Foundation/Core/Log.h"
#include "Foundation/Core/Assert.h"

#include "Foundation/Core/Timestep.h"

#include "Foundation/Core/Input.h"
#include "Foundation/Core/KeyCodes.h"
#include "Foundation/Core/MouseCodes.h"

#include "Foundation/Core/Random.h"

#include "Foundation/ImGui/ImGuiLayer.h"

#include "Foundation/Particles/ParticleSystem.h"

#include "Foundation/Scene/Scene.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/Components/Component.h"

// ----- RENDERER --------
#include "Foundation/Renderer/Renderer.h"
#include "Foundation/Renderer/Renderer2D.h"
#include "Foundation/Renderer/RenderCommand.h"

#include "Foundation/Renderer/Buffer.h"
#include "Foundation/Renderer/FrameBuffer.h"
#include "Foundation/Renderer/Shader.h"
#include "Foundation/Renderer/Texture.h"
#include "Foundation/Renderer/Model.h"
#include "Foundation/Renderer/SubTexture2D.h"
#include "Foundation/Renderer/VertexArray.h"

#include "Foundation/Renderer/OrthographicCamera.h"
#include "Foundation/Renderer/OrthographicCameraController.h"
// -----------------------

// ----- NETWORKING ------
#include "Foundation/Network/Client.h"
#include "Foundation/Network/Server.h"
// -----------------------