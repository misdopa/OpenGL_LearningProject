#Shader VERTEX
#version 330 core

layout(location=0) in vec4 position;
layout(location = 1) in vec2 textCoord;

uniform mat4 u_MVP;

out vec2 v_TextCoord;

void main()
{
   gl_Position = u_MVP*position;
   v_TextCoord = textCoord;
}


#Shader FRAGMENT
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_TextCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TextCoord);//堆纹理坐标的值进行采样？
   color = texColor;
}