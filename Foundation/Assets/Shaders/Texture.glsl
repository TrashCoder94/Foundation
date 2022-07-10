// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 m_Position;
layout(location = 1) in vec2 m_TexCoord;
layout(location = 2) in vec4 m_Colour;
layout(location = 3) in float m_TexIndex;
layout(location = 4) in float m_TilingFactor;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Colour;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_TexCoord = m_TexCoord;
	v_Colour = m_Colour;
	v_TexIndex = m_TexIndex;
	v_TilingFactor = m_TilingFactor;
	gl_Position = u_ViewProjection * vec4(m_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 m_Colour;

uniform sampler2D u_Textures[32];

in vec2 v_TexCoord;
in vec4 v_Colour;
in float v_TexIndex;
in float v_TilingFactor;

void main()
{
	//m_Colour = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Colour;
	vec4 texColour = v_Colour;
	switch(int(v_TexIndex))
	{
		case 0: texColour *= texture(u_Textures[0], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 1: texColour *= texture(u_Textures[1], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 2: texColour *= texture(u_Textures[2], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 3: texColour *= texture(u_Textures[3], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 4: texColour *= texture(u_Textures[4], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 5: texColour *= texture(u_Textures[5], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 6: texColour *= texture(u_Textures[6], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 7: texColour *= texture(u_Textures[7], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 8: texColour *= texture(u_Textures[8], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 9: texColour *= texture(u_Textures[9], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 10: texColour *= texture(u_Textures[10], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 11: texColour *= texture(u_Textures[11], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 12: texColour *= texture(u_Textures[12], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 13: texColour *= texture(u_Textures[13], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 14: texColour *= texture(u_Textures[14], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 15: texColour *= texture(u_Textures[15], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 16: texColour *= texture(u_Textures[16], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 17: texColour *= texture(u_Textures[17], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 18: texColour *= texture(u_Textures[18], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 19: texColour *= texture(u_Textures[19], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 20: texColour *= texture(u_Textures[20], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 21: texColour *= texture(u_Textures[21], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 22: texColour *= texture(u_Textures[22], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 23: texColour *= texture(u_Textures[23], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 24: texColour *= texture(u_Textures[24], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 25: texColour *= texture(u_Textures[25], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 26: texColour *= texture(u_Textures[26], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 27: texColour *= texture(u_Textures[27], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 28: texColour *= texture(u_Textures[28], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 29: texColour *= texture(u_Textures[29], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 30: texColour *= texture(u_Textures[30], v_TexCoord * v_TilingFactor) * v_Colour; break;
		case 31: texColour *= texture(u_Textures[31], v_TexCoord * v_TilingFactor) * v_Colour; break;
	}
	m_Colour = texColour;
}