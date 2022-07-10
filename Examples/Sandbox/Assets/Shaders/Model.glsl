// Basic Model Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 m_Position;
layout(location = 1) in vec3 m_Normal;
layout(location = 2) in vec2 m_TextureCoordinates;
layout(location = 3) in vec3 m_Tangent;
layout(location = 4) in vec3 m_Bitangent;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = m_TextureCoordinates;
	gl_Position = u_ViewProjection * u_Transform * vec4(m_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 m_Colour;

uniform sampler2D texture_diffuse1;

in vec2 v_TexCoord;

void main()
{
	m_Colour = texture(texture_diffuse1, v_TexCoord);
}